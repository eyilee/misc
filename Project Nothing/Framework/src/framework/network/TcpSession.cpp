#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/InStream.h"
#include "framework/network/NetBridge.h"
#include "framework/network/OutStream.h"
#include "framework/network/TcpSession.h"

CTcpSession::CTcpSession (tcp::socket& _rkSocket)
	: m_kSocket (std::move (_rkSocket))
	, m_kWriteBuffer {}
	, m_kReadBuffer {}
	, m_pkNetBridge (nullptr)
{
}

CTcpSession::~CTcpSession ()
{
}

void CTcpSession::init ()
{
	m_pkNetBridge = std::make_shared<CNetBridge> (shared_from_this ());

	async_read ();
}

void CTcpSession::shutdown ()
{
	auto self (shared_from_this ());
	m_kSocket.async_wait (tcp::socket::wait_read, [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			m_kSocket.shutdown (tcp::socket::shutdown_type::shutdown_both);
			m_kSocket.close ();
		});
}

void CTcpSession::async_read ()
{
	auto self (shared_from_this ());
	m_kSocket.async_read_some (boost::asio::buffer (m_kReadBuffer, TCP_SESSION_BUFFER_SIZE),
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
			else {
				on_read (boost::asio::buffer (m_kReadBuffer, _nLength));
			}

			async_read ();
		});
}

void CTcpSession::async_write (std::size_t _nBytes)
{
	auto self (shared_from_this ());
	boost::asio::async_write (m_kSocket, boost::asio::buffer (m_kWriteBuffer, _nBytes),
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
		});
}

void CTcpSession::on_read (const boost::asio::const_buffer& _rkBuffer)
{
	const char* buffer = boost::asio::buffer_cast<const char*> (_rkBuffer);
	CInStream inStream (buffer, _rkBuffer.size ());

	m_pkNetBridge->resolve_input (inStream);
}

void CTcpSession::on_write (const COutStream& _rkOutStream)
{
	const std::vector<char>& data = _rkOutStream.data ();
	std::copy (data.begin (), data.end (), m_kWriteBuffer.begin ());

	async_write (data.size ());
}
