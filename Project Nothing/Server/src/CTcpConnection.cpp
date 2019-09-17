#include "stdafx.h"

#include "CTcpConnection.h"

CTcpConnection::CTcpConnection (tcp::socket _socket)
	: m_kSocket (std::move (_socket))
	, m_kSend_buffer ()
	, m_kReceive_buffer ()
{
}

CTcpConnection::~CTcpConnection ()
{
}

void CTcpConnection::Init ()
{
	do_read ();
}

void CTcpConnection::do_read ()
{
	auto self (shared_from_this ());
	m_kSocket.async_read_some (asio::buffer (m_kReceive_buffer, 1024),
		[this, self](std::error_code error, std::size_t length)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
			else {
				do_write (length);
			}
		});
}

void CTcpConnection::do_write (std::size_t length)
{
	auto self (shared_from_this ());
	asio::async_write (m_kSocket, asio::buffer (m_kReceive_buffer, length),
		[this, self](std::error_code error, std::size_t /*length*/)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
			else {
				do_read ();
			}
		});
}