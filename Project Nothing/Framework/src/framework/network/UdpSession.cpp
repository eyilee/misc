#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/Random.h"
#include "framework/network/UdpConnection.h"
#include "framework/network/UdpSession.h"

using boost::asio::ip::udp;

CUdpSession::SSendCommand::SSendCommand (const std::vector<uint8_t>& _rkBytes)
	: m_kBytes (_rkBytes)
{
}

CUdpSession::SSendCommand::~SSendCommand ()
{
}

CUdpSession::CUdpSession (udp::socket& _rkSocket)
	: m_nKey (CRandom::GetValue<uint32_t> ())
	, m_kSocket (std::move (_rkSocket))
	, m_kReceiveBuffer {}
{
}

CUdpSession::~CUdpSession ()
{
}

void CUdpSession::Init (std::shared_ptr<IUdpConnection> _pkUdpConnection)
{
	m_pkConnection = _pkUdpConnection;

	AsyncReceive ();
}

void CUdpSession::Shutdown ()
{
	if (!m_kSocket.is_open ())
	{
		m_pkConnection = nullptr;
		return;
	}

	auto self (shared_from_this ());
	m_kSocket.async_wait (udp::socket::wait_read, [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			m_kSocket.shutdown (udp::socket::shutdown_type::shutdown_both);
			m_kSocket.close ();

			m_pkConnection->OnDisconnect ();
			m_pkConnection = nullptr;
		});
}

void CUdpSession::Send (const CBitOutStream& _rkOutStream)
{
	size_t size = _rkOutStream.GetSize ();
	if (size == 0 || size > UDP_SOCKET_BUFFER_SIZE) {
		LOG_ERROR ("Bytes size(%llu) is 0 or more than %llu.", size, UDP_SOCKET_BUFFER_SIZE);
		return;
	}

	bool isSending = !m_kSendQueue.empty ();

	// TODO: udp key
	m_kSendQueue.emplace_back (_rkOutStream.GetBytes ());

	if (!isSending) {
		AsyncSend ();
	}
}

void CUdpSession::AsyncReceive ()
{
	auto self (shared_from_this ());
	m_kSocket.async_receive (boost::asio::buffer (m_kReceiveBuffer, UDP_SOCKET_BUFFER_SIZE),
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode)
			{
				LOG_ERROR (_rkErrorCode.message ());

				Shutdown ();
			}
			else
			{
				if (_nLength > 0) {
					OnReceive (_nLength);
				}

				AsyncReceive ();
			}
		});
}

void CUdpSession::OnReceive (size_t _nLength)
{
	CBitInStream inStream (&m_kReceiveBuffer[0], _nLength);
	m_pkConnection->ResolveInput (inStream);
}

void CUdpSession::AsyncSend ()
{
	if (m_kSendQueue.empty ()) {
		return;
	}

	const SSendCommand& command = m_kSendQueue.front ();

	auto self (shared_from_this ());
	m_kSocket.async_send (boost::asio::buffer (command.m_kBytes, command.m_kBytes.size ()),
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			m_kSendQueue.pop_front ();

			AsyncSend ();
		});
}
