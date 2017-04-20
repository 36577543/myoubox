#pragma once
#include "Logger.h"
#include "Message.pb.h"
#include "vr.pb.h"

#include <mutex>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/chrono/chrono.hpp>

const unsigned int WM_APP_CENTER_EVENT = WM_APP + 1000;
const unsigned int CENTER_EVENT_USER_LOGIN = 1;


class TcpClient;
class TcpSession;
class Communication
{
public:
	static Communication& getInstance();

	bool setConfig(const std::string& peerAddr	// 对方地址
				, const std::string& peerPort);	// 对方端口
	std::string GetLastError();

	void bgnBusiness();
	void endBusiness();
	void setWnd(HWND hwnd);

	bool authentication();
	bool getConfig();
	bool getPeriodPriceList();
	bool beginCharge(const std::string& openid);
	bool checkout(const std::string& openid);

	void handleRequest(std::shared_ptr<TcpSession> session, Message msg);	// 处理网吧客户端的请求

	Config _centerConfig;
	PeriodPriceList _priceList;
	Bill _bill;
	UserInfo _user;
	HWND _hwnd;

	int64_t _storeID;
	std::string _deviceID;
private:
	Communication();
	~Communication();

	std::mutex _mutex;
	boost::asio::ip::tcp::resolver::iterator _peerAddrIter;

	std::thread _threadNet;
	std::thread _threadBiz;
	boost::asio::io_service _iosNet;
	boost::asio::io_service _iosBiz;
	std::shared_ptr<boost::asio::io_service::work> _workNet;
	std::shared_ptr<boost::asio::io_service::work> _workBiz;

	boost::chrono::seconds _timeoutCall;
	std::shared_ptr<TcpClient> _tcpClient;
	boost::asio::deadline_timer _timer;

	std::string _lastError;
	src::severity_channel_logger<SeverityLevel> _logger;
};

inline void Communication::setWnd(HWND hwnd)
{
	_hwnd = hwnd;
}

inline std::string Communication::GetLastError()
{
	return _lastError;
}
