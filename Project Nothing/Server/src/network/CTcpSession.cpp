#include "stdafx.h"

#include "CNetBridge.h"
#include "CTcpSession.h"

CTcpSession::CTcpSession (tcp::socket& _socket)
	: m_kSocket (std::move (_socket))
	, m_kSend_buffer ()
	, m_kReceive_buffer ()
{
	m_pNet_bridge = std::make_shared<CNetBridge> (shared_from_this ());
}

CTcpSession::~CTcpSession ()
{
}

void CTcpSession::init ()
{
	async_read ();

	std::cout << "TcpSession init, connection establish." << std::endl;
}

void CTcpSession::async_read ()
{
	auto self (shared_from_this ());
	m_kSocket.async_read_some (asio::buffer (m_kReceive_buffer, 1024),
		[this, self](std::error_code error, std::size_t length)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
			else {
				async_write (length);
			}
		});
}

void CTcpSession::async_write (std::size_t length)
{
	auto self (shared_from_this ());
	asio::async_write (m_kSocket, asio::buffer (m_kReceive_buffer, length),
		[this, self](std::error_code error, std::size_t /*length*/)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
			else {
				async_read ();
			}
		});
}