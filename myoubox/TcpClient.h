#pragma once
#include "TcpSession.h"
#include "Logger.h"

class TcpClient
{
public:
	TcpClient(boost::asio::io_service& ios);
	virtual ~TcpClient();
	void setPeerAddress(boost::asio::ip::tcp::resolver::iterator iter);

	bool syncConnect();
	void asyncConnect();
	std::function<void(void)> _authentication = NULL;// 连接后身份认证

	SessionPtr session();
	void setAutoReconnect(bool autoReconnect);
	bool isConnected();
	void stop();
	void disconnect();

private:
	boost::asio::ip::tcp::resolver::iterator _endpointIter;
	boost::asio::deadline_timer _reconnectTimer;	// 重连定时器
	int _reconnectInterval;							// 重连时间间隔（秒）：2，4，8，16，32，64...最多不大于10分钟

	SessionPtr _session;
	src::severity_channel_logger<SeverityLevel> _logger;
};

inline void TcpClient::setPeerAddress(boost::asio::ip::tcp::resolver::iterator iter)
{
	_endpointIter = iter;
}

inline void TcpClient::stop()
{
	_session->stopSession();
}

inline void TcpClient::disconnect()
{
	boost::system::error_code err;
	auto &socket = _session->socket();
	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
	socket.close(err);
}

inline SessionPtr TcpClient::session()
{
	return _session;
}

inline bool TcpClient::isConnected()
{
	return _session->isConnected();
}

inline void TcpClient::setAutoReconnect(bool autoReconnect)
{
	if (!autoReconnect)
		_session->_afterNetError = NULL;
	else
		_session->_afterNetError = [this]() {asyncConnect();};
}

