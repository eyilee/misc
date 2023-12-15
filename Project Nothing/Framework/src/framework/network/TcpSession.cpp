#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitConverter.h"
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
	m_kSocket.async_read_some (boost::asio::buffer (m_kReadBuffer, TCP_SOCKET_BUFFER_SIZE),
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
				if (_nLength > 0) {
					OnRead (_nLength);
				}

				AsyncRead ();
			}
		});
}

void CTcpSession::OnRead (const size_t& _rnLength)
{
	size_t offset = 0;
	size_t tail = _rnLength;

	if (!m_kReadQueue.empty ())
	{
		SReadCommand& command = m_kReadQueue.back ();
		if (command.m_nSize > command.m_nByteOffset)
		{
			size_t step = std::min (command.m_nSize, tail - offset);
			std::copy (m_kReadBuffer.begin () + offset, m_kReadBuffer.begin () + offset + step, command.m_kBytes.begin () + command.m_nByteOffset);
			offset += step;
			command.m_nByteOffset += step;
		}
	}

	while (offset < tail)
	{
		size_t size = ntohs (BitConverter::ToUInt16 (&m_kReadBuffer[offset]));
		if (size == 0) {
			break;
		}

		offset += sizeof (uint16_t);

		SReadCommand& command = m_kReadQueue.emplace_back (size);

		size_t step = std::min (size, tail - offset);
		std::copy (m_kReadBuffer.begin () + offset, m_kReadBuffer.begin () + offset + step, command.m_kBytes.begin () + command.m_nByteOffset);
		offset += step;
		command.m_nByteOffset += step;
	}

	while (!m_kReadQueue.empty ())
	{
		SReadCommand& command = m_kReadQueue.front ();
		if (command.m_nSize > command.m_nByteOffset) {
			break;
		}

		CBitInStream inStream (command.m_kBytes);
		m_pkNetBridge->ResolveInput (inStream);

		m_kReadQueue.pop_front ();
	}
}

void CTcpSession::AsyncWrite ()
{
	if (m_kWriteQueue.empty ()) {
		return;
	}

	SWriteCommand& command = m_kWriteQueue.front ();

	auto self (shared_from_this ());
	m_kSocket.async_write_some (boost::asio::buffer (&command.m_kBytes[command.m_nByteOffset], command.m_kBytes.size () - command.m_nByteOffset),
		[this, self, &command](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			command.m_nByteOffset += _nLength;
			if (command.m_nByteOffset >= command.m_kBytes.size ()) {
				m_kWriteQueue.pop_front ();
			}

			AsyncWrite ();
		});
}

void CTcpSession::OnWrite (CBitOutStream& _rkOutStream)
{
	size_t size = _rkOutStream.GetSize ();
	if (size == 0 || size > TCP_SOCKET_BUFFER_SIZE) {
		LOG_ERROR ("Bytes size(%llu) is 0 or more than %llu.", size, TCP_SOCKET_BUFFER_SIZE);
		return;
	}

	bool isSending = !m_kWriteQueue.empty ();

	m_kWriteQueue.emplace_back (_rkOutStream);

	if (!isSending) {
		AsyncWrite ();
	}
}
