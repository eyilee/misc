#include "stdafx.h"

#include "framework/network/CInStream.h"
#include "framework/network/CNetBridge.h"
#include "framework/network/COutStream.h"
#include "framework/network/CTcpSession.h"

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
}

void CTcpSession::async_read ()
{
	auto self (shared_from_this ());
	m_kSocket.async_read_some (boost::asio::buffer (m_kReceive_buffer, 1024),
		[this, self](boost::system::error_code error, std::size_t length)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
			else
			{
				on_read (boost::asio::buffer (m_kReceive_buffer, length));
				async_read ();
			}
		});
}

void CTcpSession::async_write (std::size_t _nLength)
{
	auto self (shared_from_this ());
	boost::asio::async_write (m_kSocket, boost::asio::buffer (m_kSend_buffer, _nLength),
		[this, self](boost::system::error_code error, std::size_t /*length*/)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
		});
}

void CTcpSession::on_read (const boost::asio::const_buffer& _kBuffer)
{
	const char* pBuffer = boost::asio::buffer_cast<const char*>(_kBuffer);
	CInStream kIn_stream (pBuffer, _kBuffer.size ());

	m_pNet_bridge->resolve_input (kIn_stream);
}

void CTcpSession::on_write (const COutStream& _kOut_stream)
{
	const std::vector<char>& kData = _kOut_stream.data ();
	std::copy (kData.begin (), kData.end (), m_kSend_buffer.begin ());

	async_write (kData.size ());
}
