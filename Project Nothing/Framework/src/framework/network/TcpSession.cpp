#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetBridge.h"
#include "framework/manager/EntityManager.h"
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

void CTcpSession::Init ()
{
	if (!m_kSocket.is_open ()) {
		return;
	}

	if (m_pkNetBridge != nullptr) {
		return;
	}

	m_pkNetBridge = std::make_shared<CNetBridge> (shared_from_this (), m_kSocket.remote_endpoint ().address ().to_v4 ().to_uint ());

	AsyncRead ();
}

void CTcpSession::Shutdown ()
{
	if (m_kSocket.is_open ())
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

	if (m_pkNetBridge != nullptr)
	{
		std::shared_ptr<IEntity> entity = m_pkNetBridge->GetEntity ();
		if (entity != nullptr) {
			entity->SetNetBridge (nullptr);
		}

		m_pkNetBridge->SetEntity (nullptr);
	}
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

void CTcpSession::AsyncWrite (std::size_t _nBytes)
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

void CTcpSession::OnRead (const boost::asio::const_buffer& _rkBuffer)
{
	const uint8_t* buffer = boost::asio::buffer_cast<const uint8_t*> (_rkBuffer);
	CBitInStream inStream (buffer, _rkBuffer.size ());

	m_pkNetBridge->ResolveInput (inStream);
}

void CTcpSession::OnWrite (const CBitOutStream& _rkOutStream)
{
	const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
	std::copy (bytes.begin (), bytes.end (), m_kWriteBuffer.begin ());

	AsyncWrite (bytes.size ());
}
