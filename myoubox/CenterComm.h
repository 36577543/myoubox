#pragma once
#include "Logger.h"
#include "Message.pb.h"
#include "vr.pb.h"

#include <mutex>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/ip/tcp.hpp>

const unsigned int WM_APP_CENTER_EVENT = WM_APP + 1000;
const unsigned int CENTER_EVENT_USER_LOGIN = 1;

class TcpClient;
class TcpSession;
class CenterComm
{
public:
	static CenterComm& getInstance();

	void setConfig(					// 设置参数
		const std::string& peerAddr	// 对方地址（广告中心地址或网吧服务端地址）
		, int peerPort				// 对方端口
		, int storeId				// 营业点ID
		, const std::string& logLvl);// 日志级别

	void setLockWnd(HWND hwnd);// 设置锁屏窗口句柄

	void bgnBusiness();

	void endBusiness();

	bool authentication();

	void GetConfig();

	void GetPricePolicy();

	void Checkout(std::string openid);

	void handleRequest(std::shared_ptr<TcpSession> session, Message msg);	// 处理网吧客户端的请求

	HWND _hwnd;
	Config _config;
	UserInfo _userinfo;
	PricePolicy _pricePolicy;

private:
	CenterComm();
	~CenterComm();

	std::mutex _mutex;
	int _storeId;
	std::string _deviceId;
	boost::asio::ip::tcp::endpoint _endpoint;

	std::thread _threadNet;
	std::thread _threadBiz;
	boost::asio::io_service _iosNet;
	boost::asio::io_service _iosBiz;
	std::shared_ptr<boost::asio::io_service::work> _workNet;
	std::shared_ptr<boost::asio::io_service::work> _workBiz;

	std::shared_ptr<TcpClient> _tcpClient;

	src::severity_channel_logger<SeverityLevel> _logger;
};

inline void CenterComm::setLockWnd(HWND hwnd)
{
	_hwnd = hwnd;
}