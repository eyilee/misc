#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/UdpConnection.h"

CUdpConnection::SHeader::SHeader ()
{
}

CUdpConnection::SHeader::~SHeader ()
{
}

void CUdpConnection::SHeader::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nSequence);
	_rkOutStream.Write (m_nAck);
	_rkOutStream.Write (m_nAckBits);
}

void CUdpConnection::SHeader::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nSequence);
	_rkInStream.Read (m_nAck);
	_rkInStream.Read (m_nAckBits);
}

CUdpConnection::CUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CUdpSession> _pkUdpSession)
	: m_pkNetBridge (_pkNetBridge)
	, m_pkUdpSession (_pkUdpSession)
	, m_kLocalEndPoint (_pkUdpSession->GetLocalEndpoint ())
	, m_nInSequence (0)
	, m_nInAckBits (0)
	, m_nOutSequence (1)
	, m_nOutAck (0)
	, m_nOutAckBits (0)
{
}

CUdpConnection::~CUdpConnection ()
{
}

void CUdpConnection::Init ()
{
	m_pkUdpSession->Init (shared_from_this ());
}

void CUdpConnection::Shutdown ()
{
	m_pkNetBridge = nullptr;

	m_pkUdpSession->Shutdown ();
	m_pkUdpSession = nullptr;
}

void CUdpConnection::OnDisconnect ()
{
	m_pkUdpSession = nullptr;
}

void CUdpConnection::ResolveInput (CBitInStream& _rkInStream)
{
	uint32_t id;
	uint32_t key;
	_rkInStream.Read (id);
	_rkInStream.Read (key);

	if (id != m_pkNetBridge->GetID () || key != m_pkNetBridge->GetKey ()) {
		return;
	}

	uint32_t sequence = ResolveHeader (_rkInStream);
	if (sequence == 0) {
		return;
	}

	unsigned short protocolID;
	_rkInStream.Read (protocolID);

	std::shared_ptr<INetProtocol> protocol = CProtocolManager::GenerateProtocol (protocolID);
	if (protocol != nullptr)
	{
		protocol->SetNetBridge (m_pkNetBridge);
		protocol->Deserialize (_rkInStream);
		protocol->Excute ();
	}
}

void CUdpConnection::ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol)
{
	if (m_kOutPackets.IsExist (m_nOutSequence)) {
		// TODO: need client send data to update ack
		return;
	}

	SHeader header;
	header.m_nSequence = m_nOutSequence;
	header.m_nAck = m_nInSequence;
	header.m_nAckBits = m_nInAckBits;

	SOutPacket& outPacket = m_kOutPackets.Insert (m_nOutSequence);
	// TODO: reliable packet

	CBitOutStream outStream;
	outStream.Write (m_pkNetBridge->GetID ());
	outStream.Write (m_pkNetBridge->GetKey ());
	outStream.Write (header);
	_pkProtocol->OnSerialize (outStream);
	m_pkUdpSession->Send (outStream);

	// NOTE: in an extreme case sequence may overflow
	m_nOutSequence++;
}

uint32_t CUdpConnection::ResolveHeader (CBitInStream& _rkInStream)
{
	SHeader header;
	_rkInStream.Read (header);

	uint32_t newInSequence = header.m_nSequence;
	uint32_t newOutAck = header.m_nAck;
	uint32_t newOutAckBits = header.m_nAckBits;

	if (newInSequence > m_nInSequence)
	{
		uint32_t distance = newInSequence - m_nInSequence;
		if (distance > 31) {
			m_nInAckBits = 1;
		}
		else
		{
			m_nInAckBits <<= distance;
			m_nInAckBits |= 1;
		}

		m_nInSequence = newInSequence;
	}
	else if (newInSequence < m_nInSequence)
	{
		uint32_t distance = m_nInSequence - newInSequence;
		if (distance > 31) {
			return 0;
		}

		uint32_t ackBit = 1 << distance;
		if ((m_nInAckBits & ackBit) != 0) {
			return 0;
		}

		m_nInAckBits |= ackBit;
	}
	else {
		return 0;
	}

	if (newOutAck <= m_nOutAck) {
		return newInSequence;
	}

	uint32_t latestLostSequence = m_nOutAck >= 31 ? m_nOutAck - 31 : 0;
	uint32_t oldestLostSequence = newOutAck >= 31 ? newOutAck - 31 : 0;
	for (uint32_t sequence = oldestLostSequence; sequence < latestLostSequence; sequence++)
	{
		uint32_t distance = m_nOutAck >= sequence ? m_nOutAck - sequence : 0;
		uint32_t ackBit = 1 << distance;
		bool hasAcked = (m_nOutAckBits & ackBit) != 0;

		if (m_kOutPackets.IsExist (sequence) && !hasAcked)
		{
			SOutPacket* packet = m_kOutPackets.TryGet (sequence);
			if (packet != nullptr)
			{
				OnPacketAcked (sequence, packet);
			}

			m_kOutPackets.Remove (sequence);
		}
	}

	m_nOutAck = newOutAck;
	m_nOutAckBits = newOutAckBits;

	uint32_t latestSequence = m_nOutAck;
	uint32_t oldestSequence = m_nOutAck >= 31 ? m_nOutAck - 31 : 0;
	for (uint32_t sequence = oldestSequence; sequence <= latestSequence; sequence++)
	{
		uint32_t distance = m_nOutAck - sequence;
		uint32_t ackBit = 1 << distance;
		bool hasAcked = (m_nOutAckBits & ackBit) != 0;

		if (m_kOutPackets.IsExist (sequence) && !hasAcked)
		{
			SOutPacket* packet = m_kOutPackets.TryGet (sequence);
			if (packet != nullptr)
			{
				OnPacketAcked (sequence, packet);
			}

			m_kOutPackets.Remove (sequence);
		}
	}

	return newInSequence;
}

void CUdpConnection::OnPacketAcked (uint32_t _nSequence, SOutPacket* _pkOutPacket)
{
	// TODO: on acked
}
