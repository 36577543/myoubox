#include "TcpClient.h"
#include <boost/asio/connect.hpp>		// boost::asio::async_connect

TcpClient::TcpClient(boost::asio::io_service& ios):
	_reconnectTimer(ios),
	_reconnectInterval(1),
	_session(new TcpSession(ios)),
	_logger(keywords::channel = "net")
{
	_session->_afterNetError = [this](){
		this->asyncConnect();
	};
}

TcpClient::~TcpClient()
{
	_session->stopSession();
}

bool TcpClient::syncConnect()
{
	boost::system::error_code ec;
	boost::asio::connect(_session->socket(), _endpointIter, ec);
	if (!ec)
	{
		_session->startSession();
		if (_authentication)
			_authentication();
		return true;
	}
	else
	{
		LOG_ERROR(_logger) << "syncConnect error:" << ec.value() << "	" << ec.message();
		return false;
	}
}

void TcpClient::asyncConnect()
{
	if (_reconnectInterval < 10 * 60)
		_reconnectInterval *= 2;

	boost::asio::async_connect(_session->socket(), _endpointIter,
		[this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator i)
	{
		if (!ec)
		{
			_reconnectInterval = 1;
			_session->startSession();
			if (_authentication)
				_authentication();
		}
		else
		{
			BOOST_LOG_NAMED_SCOPE("handle connect");
			LOG_NTFY(_logger) << logging::add_value("ErrorCode", ec.value()) << ec.message();

			_reconnectTimer.expires_from_now(
				boost::posix_time::seconds(_reconnectInterval));

			_reconnectTimer.async_wait(
				boost::bind(&TcpClient::asyncConnect, this));
		}
	});
}
