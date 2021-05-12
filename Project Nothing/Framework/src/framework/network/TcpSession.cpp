#include "stdafx.h"

#include "framework/network/InStream.h"
#include "framework/network/NetBridge.h"
#include "framework/network/OutStream.h"
#include "framework/network/TcpSession.h"

CTcpSession::CTcpSession (tcp::socket& _kSocket)
	: m_kSocket (std::move (_kSocket))
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
	m_pNet_bridge = std::make_shared<CNetBridge> (shared_from_this ());

	async_read ();
}

void CTcpSession::shutdown ()
{
	auto self (shared_from_this ());
	m_kSocket.async_wait (tcp::socket::wait_read, [this, self](const boost::system::error_code& _kError_code)
		{
			if (_kError_code) {
				LOG_ERROR (_kError_code.message ());
			}
			else
			{
				m_kSocket.shutdown (tcp::socket::shutdown_type::shutdown_both);
				m_kSocket.close ();
			}
		});
}

void CTcpSession::async_read ()
{
	auto self (shared_from_this ());
	m_kSocket.async_read_some (boost::asio::buffer (m_kReceive_buffer, 1024),
		[this, self](const boost::system::error_code& _kError_code, std::size_t _nLength)
		{
			if (_kError_code) {
				LOG_ERROR (_kError_code.message ());
			}
			else
			{
				on_read (boost::asio::buffer (m_kReceive_buffer, _nLength));
				async_read ();
			}
		});
}

void CTcpSession::async_write (std::size_t _nLength)
{
	auto self (shared_from_this ());
	boost::asio::async_write (m_kSocket, boost::asio::buffer (m_kSend_buffer, _nLength),
		[this, self](const boost::system::error_code& _kError_code, std::size_t)
		{
			if (_kError_code) {
				LOG_ERROR (_kError_code.message ());
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