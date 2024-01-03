#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/Entity.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"
#include "framework/manager/EntityManager.h"
#include "framework/network/UdpSession.h"

CUdpSession::CUdpSession (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort)
	: m_kSocket (_rkContext, udp::endpoint (boost::asio::ip::address::from_string (_rkHostAddr.c_str ()), _nPort))
	, m_kReceiveBuffer {}
{
}

CUdpSession::~CUdpSession ()
{
}

void CUdpSession::Init ()
{
	AsyncReceive ();
}

void CUdpSession::Shutdown ()
{
	if (!m_kSocket.is_open ()) {
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
		});
}

void CUdpSession::AsyncReceive ()
{
	auto self (shared_from_this ());
	m_kSocket.async_receive_from (boost::asio::buffer (m_kReceiveBuffer, UDP_SOCKET_BUFFER_SIZE), m_kEndpoint,
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

	int entityID;
	uint32_t key;
	inStream.Read (entityID);
	inStream.Read (key);

	std::shared_ptr<IEntity> entity = CEntityManager::GetEntity (entityID);
	if (entity == nullptr) {
		return;
	}

	std::shared_ptr<CNetBridge> netBridge = entity->GetNetBridge ();
	if (netBridge == nullptr) {
		return;
	}

	uint32_t ip = m_kEndpoint.address ().to_v4 ().to_uint ();
	if (netBridge->GetIP () == ip && netBridge->GetKey () == key) {
		netBridge->ResolveInput (inStream);
	}
}

void CUdpSession::AsyncSend ()
{
	if (m_kSendQueue.empty ()) {
		return;
	}

	const SSendCommand& command = m_kSendQueue.front ();

	auto self (shared_from_this ());
	m_kSocket.async_send_to (boost::asio::buffer (command.m_kBytes, command.m_kBytes.size ()), command.m_kEndPoint,
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			m_kSendQueue.pop_front ();

			AsyncSend ();
		});
}

void CUdpSession::OnSend (const CBitOutStream& _rkOutStream, const udp::endpoint& _rkEndPoint)
{
	size_t size = _rkOutStream.GetSize ();
	if (size == 0 || size > UDP_SOCKET_BUFFER_SIZE) {
		LOG_ERROR ("Bytes size(%llu) is 0 or more than %llu.", size, UDP_SOCKET_BUFFER_SIZE);
		return;
	}

	bool isSending = !m_kSendQueue.empty ();

	m_kSendQueue.emplace_back (_rkOutStream.GetBytes (), _rkEndPoint);

	if (!isSending) {
		AsyncSend ();
	}
}
