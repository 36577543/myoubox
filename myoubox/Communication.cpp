#include "Communication.h"
#include "TcpSession.h"
#include "TcpClient.h"
//#include "TcpServer.h"
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>	// boost::to_upper_copy
#include "boost/date_time/posix_time/posix_time.hpp"
#include "md5.hh"

#include "SvrConfig.h"
#include "Environ.h"

using std::once_flag;
using std::call_once;
using std::mutex;
using std::unique_lock;

Communication::Communication():
	_timeoutCall(10),
	_timer(_iosBiz),
	_logger(keywords::channel = "Comm")
{
	_workNet.reset(new boost::asio::io_service::work(_iosNet));
	_workBiz.reset(new boost::asio::io_service::work(_iosBiz));
	_threadNet = std::thread([this]() {_iosNet.run(); });
	_threadBiz = std::thread([this]() {_iosBiz.run(); });
	_tcpClient.reset(new TcpClient(_iosNet));
}

Communication::~Communication()
{
	endBusiness();
	google::protobuf::ShutdownProtobufLibrary();
}

Communication& Communication::getInstance()
{
	static once_flag instanceFlag;
	static Communication* pInstance;

	call_once(instanceFlag, []()
	{
		static Communication instance;
		pInstance = &instance;
	});
	return *pInstance;
}

bool Communication::setConfig(const std::string& peerAddr, const std::string& peerPort)
{
	_storeID = SvrConfig::getInstance()._storeID;
	_deviceID = SvrConfig::getInstance()._deviceID;

	using namespace boost::asio::ip;
	boost::system::error_code ec;
	tcp::resolver::query query(peerAddr, peerPort);
	_peerAddrIter = tcp::resolver(_iosBiz).resolve(query, ec);
	if (!ec)
	{
		LOG_DEBUG(_logger) << "中心地址(" << peerAddr << ":" << peerPort << ")" << "解析成功(" << _peerAddrIter->endpoint().address().to_string() << ":" << peerPort << ")";
		return true;
	}
	else
	{
		_lastError = "中心地址解析失败:" + ec.message();
		LOG_ERROR(_logger) << "中心地址解析失败(" << peerAddr << ":" << peerPort << ")	(" << ec.value() << ":" << ec.message() << ")";
		return false;
	}
}

void Communication::bgnBusiness()
{
	LOG_DEBUG(_logger) << "启动服务端通信";

	_iosBiz.post([this]()
	{
#ifdef _DEBUG
	boost::posix_time::seconds _reconnectInteval(5);
#else
	boost::posix_time::seconds _reconnectInteval(5 * 60);
#endif
		_tcpClient->setPeerAddress(_peerAddrIter);
		if (!_tcpClient->syncConnect())
		{
			_timer.expires_from_now(_reconnectInteval);
			_timer.async_wait([this](const boost::system::error_code& er) { bgnBusiness(); });
			return;
		}

		if (!authentication())
		{
			_timer.expires_from_now(_reconnectInteval);
			_timer.async_wait([this](const boost::system::error_code& er) { bgnBusiness(); });
		}
		else
		{
			_tcpClient->setAutoReconnect(true);
			_tcpClient->_authentication = [this]()
			{
				_iosBiz.post([this]() { authentication(); });
			};
			_tcpClient->session()->_requestHandler = [this](Message msg)
			{
				handleRequest(_tcpClient->session(), msg);
			};

			getConfig();
			getPeriodPriceList();
		}
	});
}

void Communication::endBusiness()
{
	if (_tcpClient)
		_tcpClient->stop();

	_iosNet.stop();
	_iosBiz.stop();
	_threadNet.join();
	_threadBiz.join();
}

bool Communication::authentication()
{
	BOOST_LOG_FUNCTION();

	using namespace boost::posix_time;
	std::string when = to_iso_extended_string(second_clock::local_time());
	std::size_t found = when.find('T');
	if (found != std::string::npos)
		when[found] = ' ';

	std::string data = boost::lexical_cast<std::string>(_storeID) + when + "vr123dreamer";
	MD5 ctx;
	ctx.update((unsigned char *)data.c_str(), data.length());
	ctx.finalize();

	AuthInfo info;
	info.set_storeid(_storeID);
	info.set_when(when);
	info.set_sign(boost::to_upper_copy(ctx.hex_digest()));
	std::string content = info.SerializeAsString();

	Message msgReq;
	msgReq.set_method("Authentication");
	msgReq.set_content(content);
	msgReq.set_storeid(_storeID);
	msgReq.set_deviceid(_deviceID);

	auto fut = _tcpClient->session()->request(msgReq);
	if (fut.wait_for(_timeoutCall) != boost::future_status::ready)
	{
		LOG_ERROR(_logger) << "超时";
		return false;
	}

	auto msgRsp = fut.get();
	if (msgRsp.exceptcode() != 0)
	{
		LOG_ERROR(_logger) << "异常:" << msgRsp.exceptcode() << ", " << utf8ToGB2312(msgRsp.exceptmsg());
		return false;
	}

	if (msgRsp.content() == "true")
	{
		LOG_DEBUG(_logger) << "Authentication成功";
		return true;
	}
	else
	{
		LOG_ERROR(_logger) << "Authentication失败:" << msgRsp.content();
		return false;
	}
}

bool Communication::getConfig()
{
	BOOST_LOG_FUNCTION();

	Message msgReq;
	msgReq.set_method("GetConfig");
	msgReq.set_storeid(_storeID);
	msgReq.set_deviceid(_deviceID);

	auto fut = _tcpClient->session()->request(msgReq);
	if (fut.wait_for(_timeoutCall) != boost::future_status::ready)
	{
		LOG_ERROR(_logger) << "超时";
		_tcpClient->disconnect();
		return false;
	}

	auto msgRsp = fut.get();
	if (msgRsp.exceptcode() != 0)
	{
		LOG_ERROR(_logger) << "异常:" << msgRsp.exceptcode() << ", " << utf8ToGB2312(msgRsp.exceptmsg());
		return false;
	}

	if (!_centerConfig.ParseFromString(msgRsp.content()))
	{
		LOG_ERROR(_logger) << "content parse";
		return false;
	}

	_centerConfig.set_storename(utf8ToGB2312(_centerConfig.storename()));

	SvrConfig & svrConfig = SvrConfig::getInstance();
	if (svrConfig._storeName != _centerConfig.storename())
	{
		svrConfig._storeName = _centerConfig.storename();
		svrConfig.save();
	}
	return true;
}

bool Communication::getPeriodPriceList()
{
	BOOST_LOG_FUNCTION();

	int64_t storeID = SvrConfig::getInstance()._storeID;
	std::string deviceID = SvrConfig::getInstance()._deviceID;

	Message msgReq;
	msgReq.set_method("GetPeriodPriceList");
	msgReq.set_storeid(storeID);
	msgReq.set_deviceid(deviceID);

	auto fut = _tcpClient->session()->request(msgReq);
	if (fut.wait_for(_timeoutCall) != boost::future_status::ready)
	{
		LOG_ERROR(_logger) << "超时";
		_tcpClient->disconnect();
		return false;
	}

	auto msgRsp = fut.get();
	if (msgRsp.exceptcode() != 0)
	{
		LOG_ERROR(_logger) << "异常:" << msgRsp.exceptcode() << ", " << utf8ToGB2312(msgRsp.exceptmsg());
		return false;
	}

	if (!_priceList.ParseFromString(msgRsp.content()))
	{
		LOG_ERROR(_logger) << "content parse";
		return false;
	}

	for (auto &periodprice : _priceList.periodprices())
	{
		auto pricegroup = periodprice.pricegroup();
		pricegroup.set_name(utf8ToGB2312(pricegroup.name()));

		for (auto &price : pricegroup.prices())
		{
			auto duration = price.duration();
			auto money = price.money();
		}
	}

	return true;
}

bool Communication::beginCharge(const std::string & openid)
{
	BOOST_LOG_FUNCTION();

	Message msgReq;
	msgReq.set_method("BeginCharge");
	msgReq.set_storeid(_storeID);
	msgReq.set_deviceid(_deviceID);
	msgReq.set_content(openid);

	auto fut = _tcpClient->session()->request(msgReq);
	if (fut.wait_for(_timeoutCall) != boost::future_status::ready)
	{
		LOG_ERROR(_logger) << "超时";
		_tcpClient->disconnect();
		return false;
	}

	auto msgRsp = fut.get();
	if (msgRsp.exceptcode() != 0)
	{
		LOG_ERROR(_logger) << "异常:" << msgRsp.exceptcode() << ", " << utf8ToGB2312(msgRsp.exceptmsg());
		return false;
	}

	if (msgRsp.content() == "true")
	{
		LOG_DEBUG(_logger) << "BeginCharge:" << msgRsp.content();
		return true;
	}
	else
	{
		LOG_ERROR(_logger) << "BeginCharge:" << msgRsp.content();
		return false;
	}
}

bool Communication::checkout(const std::string & openid)
{
	BOOST_LOG_FUNCTION();

	Message msgReq;
	msgReq.set_method("Checkout");
	msgReq.set_storeid(_storeID);
	msgReq.set_deviceid(_deviceID);
	msgReq.set_content(openid);

	auto fut = _tcpClient->session()->request(msgReq);
	if (fut.wait_for(_timeoutCall) != boost::future_status::ready)
	{
		LOG_ERROR(_logger) << "超时";
		_tcpClient->disconnect();
		return false;
	}

	auto msgRsp = fut.get();
	if (msgRsp.exceptcode() != 0)
	{
		LOG_ERROR(_logger) << "异常:" << msgRsp.exceptcode() << ", " << utf8ToGB2312(msgRsp.exceptmsg());
		return false;
	}

	if (_bill.ParseFromString(msgRsp.content()))
		return true;
	else
	{
		LOG_ERROR(_logger) << "content parse";
		return false;
	}
}

void Communication::handleRequest(std::shared_ptr<TcpSession> session, Message msg)
{
	if (msg.method() == "UserLogin")
	{
		LOG_DEBUG(_logger) << "UserLogin";

		if (!_user.ParseFromString(msg.content()))
		{
			msg.set_exceptcode(1);
			msg.set_exceptmsg("content parse failed");
			session->queueRspMsg(msg);
		}
		else
		{
			_user.set_nickname(utf8ToGB2312(_user.nickname()));
			PostMessage(_hwnd, WM_APP_CENTER_EVENT, (WPARAM)CENTER_EVENT_USER_LOGIN, (LPARAM)NULL);
		}
	}
}
