#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetBridge.h"
#include "framework/manager/EntityManager.h"
#include "framework/network/TcpSession.h"

CTcpSession::CTcpSession (tcp::socket& _rkSocket)
	: m_kSocket (std::move (_rkSocket))
	, m_kReadBuffer {}
	, m_pkNetBridge (nullptr)
{
}

CTcpSession::~CTcpSession ()
{
}

void CTcpSession::Init ()
{
	m_pkNetBridge = std::make_shared<CNetBridge> ();
	m_pkNetBridge->Init (shared_from_this (), m_kSocket.remote_endpoint ().address ().to_v4 ().to_uint ());

	AsyncRead ();
}

void CTcpSession::Shutdown ()
{
	if (m_kSocket.is_open ()) {
		return;
	}

	auto self (shared_from_this ());
	m_kSocket.async_wait (tcp::socket::wait_read, [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			m_kSocket.shutdown (tcp::socket::shutdown_type::shutdown_both);
			m_kSocket.close ();

			m_pkNetBridge->Shutdown ();
			m_pkNetBridge = nullptr;
		});
}

void CTcpSession::AsyncRead ()
{
	auto self (shared_from_this ());
	m_kSocket.async_read_some (boost::asio::buffer (m_kReadBuffer, TCP_SESSION_BUFFER_SIZE),
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode)
			{
				if (_rkErrorCode != boost::asio::error::eof && _rkErrorCode != boost::asio::error::connection_reset) {
					LOG_ERROR (_rkErrorCode.message ());
				}

				Shutdown ();
			}
			else
			{
				OnRead (boost::asio::buffer (m_kReadBuffer, _nLength));
				AsyncRead ();
			}
		});
}

void CTcpSession::AsyncWrite ()
{
	if (m_kCommandQueue.empty ()) {
		return;
	}

	SCommand& command = m_kCommandQueue.front ();

	auto self (shared_from_this ());
	m_kSocket.async_write_some (boost::asio::buffer (&command.m_kBytes[command.m_nByteOffset], command.m_kBytes.size () - command.m_nByteOffset),
		[this, self, &command](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			command.m_nByteOffset += _nLength;
			if (command.m_nByteOffset >= command.m_kBytes.size ()) {
				m_kCommandQueue.pop_front ();
			}

			AsyncWrite ();
		});
}

void CTcpSession::OnRead (const boost::asio::const_buffer& _rkBuffer)
{
	const uint8_t* buffer = boost::asio::buffer_cast<const uint8_t*> (_rkBuffer);
	CBitInStream inStream (buffer, _rkBuffer.size ());

	m_pkNetBridge->ResolveInput (inStream);
}

void CTcpSession::OnWrite (const CBitOutStream& _rkOutStream)
{
	const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();

	size_t size = bytes.size ();
	if (size == 0 || size > TCP_SESSION_BUFFER_SIZE) {
		LOG_ERROR ("Bytes size(%llu) is 0 or more than %llu.", TCP_SESSION_BUFFER_SIZE);
		return;
	}

	bool isSending = !m_kCommandQueue.empty ();

	m_kCommandQueue.emplace_back (SCommand (bytes));

	if (!isSending) {
		AsyncWrite ();
	}
}
