#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"
#include "framework/manager/EntityManager.h"
#include "framework/network/UdpSession.h"

CUdpSession::CUdpSession (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort)
	: m_kSocket (_rkContext, udp::endpoint (boost::asio::ip::address::from_string (_rkHostAddr.c_str ()), _nPort))
	, m_kSendBuffer {}
	, m_kReceiveBuffer {}
{
}

CUdpSession::~CUdpSession ()
{
}

void CUdpSession::init ()
{
	async_receive ();
}

void CUdpSession::shutdown ()
{
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

void CUdpSession::async_receive ()
{
	auto self (shared_from_this ());
	m_kSocket.async_receive_from (boost::asio::buffer (m_kReceiveBuffer, UDP_SESSION_BUFFER_SIZE), m_kEndpoint,
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
			else {
				on_receive (boost::asio::buffer (m_kReceiveBuffer, _nLength));
			}

			async_receive ();
		});
}

void CUdpSession::async_send (std::size_t _nBytes)
{
	auto self (shared_from_this ());
	m_kSocket.async_send_to (boost::asio::buffer (m_kSendBuffer, _nBytes), m_kEndpoint,
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
		});
}

void CUdpSession::on_receive (const boost::asio::const_buffer& _rkBuffer)
{
	const uint8_t* buffer = boost::asio::buffer_cast<const uint8_t*> (_rkBuffer);
	CBitInStream inStream (buffer, _rkBuffer.size ());

	uint32_t ip = m_kEndpoint.address ().to_v4 ().to_uint ();

	int entityID;
	int key;
	inStream.Read (entityID);
	inStream.Read (key);

	if (CEntityManager::Instance != nullptr) {
		std::shared_ptr<IEntity> entity = CEntityManager::Instance->get_entity (entityID);
		if (entity != nullptr) {
			std::shared_ptr<CNetBridge> netBridge = entity->get_net_bridge ();
			if (netBridge != nullptr) {
				if (netBridge->get_ip () == ip && netBridge->get_key () == key) {
					netBridge->resolve_input (inStream);
				}
			}
		}
	}
}

void CUdpSession::on_send (const CBitOutStream& _rkOutStream)
{
	const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
	std::copy (bytes.begin (), bytes.end (), m_kSendBuffer.begin ());

	async_send (bytes.size ());
}
