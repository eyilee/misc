#include "stdafx.h"

#include "CInStream.h"
#include "COutStream.h"
#include "CNetBridge.h"

#include "CTcpSession.h"

CTcpSession::CTcpSession (tcp::socket& _socket)
	: m_kSocket (std::move (_socket))
	, m_kSend_buffer ()
	, m_kReceive_buffer ()
	, m_pNet_bridge (nullptr)
{
}

CTcpSession::~CTcpSession ()
{
}

void CTcpSession::init ()
{
	auto self (shared_from_this ());
	m_pNet_bridge = std::make_shared<CNetBridge> (self);

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
			else
			{
				on_read (asio::buffer (m_kReceive_buffer, length));
				async_read ();
			}
		});
}

void CTcpSession::async_write (std::size_t _nLength)
{
	auto self (shared_from_this ());
	asio::async_write (m_kSocket, asio::buffer (m_kSend_buffer, _nLength),
		[this, self](std::error_code error, std::size_t /*length*/)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
		});
}

void CTcpSession::on_read (const asio::const_buffer& _kBuffer)
{
	const char* pBuffer = asio::buffer_cast<const char*>(_kBuffer);
	CInStream kIn_stream (pBuffer, _kBuffer.size ());

	m_pNet_bridge->resolve_input (kIn_stream);
}

void CTcpSession::on_write (const COutStream& _kOut_stream)
{
	const std::vector<char>& kData = _kOut_stream.data ();
	std::copy (kData.begin (), kData.end (), m_kSend_buffer.begin ());

	async_write (kData.size ());
}
