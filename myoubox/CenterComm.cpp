#include "stdafx.h"
#include "CenterComm.h"
#include "TcpSession.h"
#include "TcpClient.h"
#include <fstream>
#include <boost/locale.hpp>				// boost::locale::conv::from_utf
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>	// boost::to_upper_copy
#include "md5.hh"

using std::once_flag;
using std::call_once;
using std::mutex;
using std::unique_lock;

CenterComm::CenterComm():
	_logger(keywords::channel = "vr")
{
	_workNet.reset(new boost::asio::io_service::work(_iosNet));
	_workBiz.reset(new boost::asio::io_service::work(_iosBiz));
	_threadNet = std::thread([this]() {_iosNet.run(); });
	_threadBiz = std::thread([this]() {_iosBiz.run(); });
	_tcpClient.reset(new TcpClient(_iosNet));
}

CenterComm::~CenterComm()
{
	google::protobuf::ShutdownProtobufLibrary();
}

CenterComm& CenterComm::getInstance()
{
	static once_flag instanceFlag;
	static CenterComm* pInstance;

	call_once(instanceFlag, []()
	{
		static CenterComm instance;
		pInstance = &instance;
	});
	return *pInstance;
}

void CenterComm::setConfig(const std::string& peerAddr, int peerPort, int storeId, const std::string& logLvl)
{
	using namespace boost::asio::ip;
	_endpoint = tcp::endpoint(address::from_string(peerAddr), peerPort);
	_storeId = storeId;

	SeverityLevel lvl;
	if (logLvl == "trace") lvl = trace;
	else if (logLvl == "debug") lvl = debug;
	else if (logLvl == "notify") lvl = notify;
	else if (logLvl == "info") lvl = info;
	else if (logLvl == "warn") lvl = warn;
	else if (logLvl == "error") lvl = error;
	else if (logLvl == "fatal") lvl = fatal;
	else lvl = debug;

	LOG_DEBUG(_logger) << "	±¾µØ¶Ë¿Ú:";
}

void CenterComm::bgnBusiness()
{
	std::ifstream ifs("config.txt");
	if (ifs.good())
		ifs >> _storeId >> _deviceId;

	LOG_DEBUG(_logger) << "Æô¶¯CenterComm, storeId:" << _storeId << ", deviceId:" << _deviceId;

	_iosBiz.post(
		[this]()
	{
		while (!_tcpClient->syncConnect(_endpoint))
			boost::this_thread::sleep(boost::posix_time::minutes(5));

		if (!authentication())
			return;
	});
}

void CenterComm::endBusiness()
{
	if (_tcpClient)
		_tcpClient->stop();

	_iosNet.stop();
	_iosBiz.stop();
	_threadNet.join();
	_threadBiz.join();
}

void gb2312ToUTF8(Message& msg)
{
	msg.set_returnmsg(boost::locale::conv::to_utf<char>(msg.returnmsg(), "gb2312"));
}
void utf8ToGB2312(Message& msg)
{
	msg.set_returnmsg(boost::locale::conv::from_utf(msg.returnmsg(), "gb2312"));
}
void utf8ToGB2312(UserInfo& user)
{
	user.set_nickname(boost::locale::conv::from_utf(user.nickname(), "gb2312"));
}

bool CenterComm::authentication()
{
	MD5 ctx;
	std::string sign = boost::lexical_cast<std::string>(_storeId) + _deviceId + "123456";
	ctx.update((unsigned char *)sign.c_str(), sign.length());
	ctx.finalize();
	sign = ctx.hex_digest();

	Message msgReq;
	msgReq.set_method("Authentication");
	msgReq.set_content(boost::to_upper_copy(sign));

	auto fut = _tcpClient->session()->request(msgReq);
	auto msgRsp = fut.get();
	utf8ToGB2312(msgRsp);
	if (msgRsp.returncode() == 0)
	{
		return true;
	}
	else
	{
		if (msgRsp.returncode() != 0)
			LOG_ERROR(_logger) << "AuthenticationÊ§°Ü:" << msgRsp.returncode() << ", " << msgRsp.returnmsg();
		else
			LOG_ERROR(_logger) << "AuthenticationÊ§°Ü, content½âÎöÊ§°Ü";
		return false;
	}
}

void CenterComm::handleRequest(std::shared_ptr<TcpSession> session, Message msg)
{
	if (msg.method() == "UserLogin")
	{
		LOG_DEBUG(_logger) << "UserLogin";

		if (!_userinfo.ParseFromString(msg.content()))
		{
			msg.set_returncode(1);
			msg.set_returnmsg("content parse failed");
			session->queueRspMsg(msg);
		}
		else
		{
			utf8ToGB2312(_userinfo);
			PostMessage(_hwnd, WM_APP_CENTER_EVENT, (WPARAM)CENTER_EVENT_USER_LOGIN, (LPARAM)NULL);
		}

	}
}
