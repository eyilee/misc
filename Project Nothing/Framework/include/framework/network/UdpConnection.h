#pragma once
#include "framework/network/BitStream.h"
#include "framework/network/SequenceBuffer.h"

using boost::asio::ip::udp;

class CUdpSession;

constexpr uint32_t FRAGMENT_SIZE = 1420; // 1500(Ethernet MTU) - 60(IPv4 header) - 8(udp header) - 12(fragment header)
constexpr uint32_t PACKET_BUFFER_SIZE = 256;

struct SFragmentHeader : public IBitSerializable
{
	uint32_t m_nSequence;
	uint32_t m_nCount;
	uint32_t m_nIndex;
	uint32_t m_Size;

	SFragmentHeader ();
	virtual ~SFragmentHeader ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
};

struct SUdpHeader : public IBitSerializable
{
	uint32_t m_nSequence;
	uint32_t m_nAck;
	uint32_t m_nAckBits;

	SUdpHeader ();
	virtual ~SUdpHeader ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
};

class IUdpConnection : public std::enable_shared_from_this<IUdpConnection>
{
public:
	IUdpConnection ();
	virtual ~IUdpConnection ();

	virtual void Init () = 0;
	virtual void Shutdown () = 0;
	virtual void OnDisconnect () = 0;

	virtual void ResolveInput (CBitInStream& _rkInStream) = 0;
};

template<typename TOutPacket>
class CUdpConnection : public IUdpConnection
{
public:
	CUdpConnection (std::shared_ptr<CUdpSession> _pkUdpSession);
	virtual ~CUdpConnection ();

	virtual void Init () override;
	virtual void Shutdown () override;
	virtual void OnDisconnect () override;

	virtual void ResolveInput (CBitInStream& _rkInStream) override;
	virtual void ResolvePackage (CBitInStream& _rkInStream) = 0;
	virtual void OnPacketAcked (uint32_t _nSequence, TOutPacket& _rkOutPacket) = 0;

	bool CanComposeOutput ();
	void BeginComposeOutput (CBitOutStream& _rkOutStream);
	void EndComposeOutput (CBitOutStream& _rkOutStream);

protected:
	uint32_t ResolveHeader (CBitInStream& _rkInStream);
	void ComposeHeader (CBitOutStream& _rkOutStream);

protected:
	std::shared_ptr<CUdpSession> m_pkUdpSession;

	uint32_t m_nInSequence;
	uint32_t m_nInAckBits;
	uint32_t m_nOutSequence;
	uint32_t m_nOutAck;
	uint32_t m_nOutAckBits;
	SequenceBuffer<TOutPacket, PACKET_BUFFER_SIZE> m_kOutPackets;
};

template<typename TOutPacket>
inline CUdpConnection<TOutPacket>::CUdpConnection (std::shared_ptr<CUdpSession> _pkUdpSession)
	: m_pkUdpSession (_pkUdpSession)
	, m_nInSequence (0)
	, m_nInAckBits (0)
	, m_nOutSequence (1)
	, m_nOutAck (0)
	, m_nOutAckBits (0)
{
}

template<typename TOutPacket>
inline CUdpConnection<TOutPacket>::~CUdpConnection ()
{
}


template<typename TOutPacket>
inline void CUdpConnection<TOutPacket>::Init ()
{
	m_pkUdpSession->Init (shared_from_this ());
}

template<typename TOutPacket>
inline void CUdpConnection<TOutPacket>::Shutdown ()
{
	m_pkUdpSession->Shutdown ();
	m_pkUdpSession = nullptr;
}

template<typename TOutPacket>
inline void CUdpConnection<TOutPacket>::OnDisconnect ()
{
	m_pkUdpSession = nullptr;
}

template<typename TOutPacket>
void CUdpConnection<TOutPacket>::ResolveInput (CBitInStream& _rkInStream)
{
	uint32_t key;
	_rkInStream.Read (key);

	if (key != m_pkUdpSession->GetKey ()) {
		return;
	}

	uint32_t sequence = ResolveHeader (_rkInStream);
	if (sequence == 0) {
		return;
	}

	ResolvePackage (_rkInStream);
}

template<typename TOutPacket>
inline bool CUdpConnection<TOutPacket>::CanComposeOutput ()
{
	if (m_kOutPackets.IsExist (m_nOutSequence)) {
		// TODO: need client send data to update ack
		return false;
	}

	return true;
}

template<typename TOutPacket>
inline void CUdpConnection<TOutPacket>::BeginComposeOutput (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_pkUdpSession->GetKey ());

	ComposeHeader (_rkOutStream);
}

template<typename TOutPacket>
inline void CUdpConnection<TOutPacket>::EndComposeOutput (CBitOutStream& _rkOutStream)
{
	uint32_t size = static_cast<uint32_t> (_rkOutStream.GetSize ());
	if (size > FRAGMENT_SIZE) {
		uint32_t fragmentCount = size / FRAGMENT_SIZE;
		uint32_t lastFragmentSize = size % FRAGMENT_SIZE;
		if (lastFragmentSize != 0) {
			fragmentCount++;
		}
		else {
			lastFragmentSize = FRAGMENT_SIZE;
		}

		for (uint32_t i = 0; i < size; i++)
		{
			uint32_t fragmentSize = i < fragmentCount - 1 ? FRAGMENT_SIZE : lastFragmentSize;

			SFragmentHeader header;
			header.m_nSequence = m_nOutSequence;
			header.m_nCount = fragmentCount;
			header.m_nIndex = i;
			header.m_Size = fragmentSize;

			const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
			uint32_t position = i * FRAGMENT_SIZE;
			auto it = bytes.begin () + position;
			std::vector<uint8_t> fragmentBytes (it, it + fragmentSize);

			CBitOutStream outStream;
			outStream.Write (static_cast<uint8_t> (1));
			outStream.Write (header);
			outStream.Align ();
			outStream.WriteBytes (fragmentBytes, fragmentSize);
			m_pkUdpSession->Send (outStream);
		}
	}
	else {
		m_pkUdpSession->Send (_rkOutStream);
	}

	// NOTE: in an extreme case sequence may overflow
	m_nOutSequence++;
}

template<typename TOutPacket>
uint32_t CUdpConnection<TOutPacket>::ResolveHeader (CBitInStream& _rkInStream)
{
	uint8_t isFragment;
	_rkInStream.Read (isFragment);

	if (isFragment == 1)
	{
		// TODO: read fragment
	}

	SUdpHeader header;
	_rkInStream.Read (header);
	_rkInStream.Align ();

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
			TOutPacket* packet = m_kOutPackets.TryGet (sequence);
			if (packet != nullptr)
			{
				OnPacketAcked (sequence, *packet);
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
			TOutPacket* packet = m_kOutPackets.TryGet (sequence);
			if (packet != nullptr)
			{
				OnPacketAcked (sequence, *packet);
			}

			m_kOutPackets.Remove (sequence);
		}
	}

	return newInSequence;
}

template<typename TOutPacket>
void CUdpConnection<TOutPacket>::ComposeHeader (CBitOutStream& _rkOutStream)
{
	SUdpHeader header;
	header.m_nSequence = m_nOutSequence;
	header.m_nAck = m_nInSequence;
	header.m_nAckBits = m_nInAckBits;

	_rkOutStream.Write (static_cast<uint8_t> (0));
	_rkOutStream.Write (header);
	_rkOutStream.Align ();
}
