#pragma once
#include "framework/network/BitStream.h"
#include "framework/network/SequenceBuffer.h"

using boost::asio::ip::udp;

class CUdpSession;

constexpr size_t PACKET_BUFFER_SIZE = 256;
constexpr size_t FRAGMENT_BUFFER_SIZE = 32;
constexpr size_t FRAGMENT_SIZE = 10; //1420; // 1500(Ethernet MTU) - 60(IPv4 header) - 8(udp header) - 12(fragment header)
constexpr size_t REASSEMBLY_SIZE = 1024 * 32;

struct SFragmentHeader : public IBitSerializable
{
	int m_nSequence;
	int m_nCount;
	int m_nIndex;
	int m_nSize;

	SFragmentHeader ();
	virtual ~SFragmentHeader ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
};

struct SFragmentReassembly
{
	int m_nCount;
	int m_nReceivedMask;
	int m_nReceivedCount;
	std::vector<uint8_t> m_kBytes;

	SFragmentReassembly ();
	virtual ~SFragmentReassembly ();
};

struct SUdpHeader : public IBitSerializable
{
	int m_nSequence;
	int m_nAck;
	int m_nAckBits;

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
	virtual void OnPacketAcked (int _nSequence, TOutPacket& _rkOutPacket) = 0;

	bool CanComposeOutput ();
	void BeginComposeOutput (CBitOutStream& _rkOutStream);
	void EndComposeOutput (CBitOutStream& _rkOutStream);

protected:
	int ResolveHeader (CBitInStream& _rkInStream);
	void ComposeHeader (CBitOutStream& _rkOutStream);

protected:
	std::shared_ptr<CUdpSession> m_pkUdpSession;

	int m_nInSequence;
	int m_nInAckBits;
	int m_nOutSequence;
	int m_nOutAck;
	int m_nOutAckBits;
	SequenceBuffer<TOutPacket, PACKET_BUFFER_SIZE> m_kOutPackets;
	SequenceBuffer<SFragmentReassembly, FRAGMENT_BUFFER_SIZE> m_kFragmentReassembly;
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

	int sequence = ResolveHeader (_rkInStream);
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
	ComposeHeader (_rkOutStream);
}

template<typename TOutPacket>
inline void CUdpConnection<TOutPacket>::EndComposeOutput (CBitOutStream& _rkOutStream)
{
	int size = static_cast<int> (_rkOutStream.GetSize ());
	if (size > FRAGMENT_SIZE) {
		int fragmentCount = size / FRAGMENT_SIZE;
		int lastFragmentSize = size % FRAGMENT_SIZE;
		if (lastFragmentSize != 0) {
			fragmentCount++;
		}
		else {
			lastFragmentSize = FRAGMENT_SIZE;
		}

		for (int i = 0; i < fragmentCount; i++)
		{
			int fragmentSize = i < fragmentCount - 1 ? FRAGMENT_SIZE : lastFragmentSize;

			SFragmentHeader header;
			header.m_nSequence = m_nOutSequence;
			header.m_nCount = fragmentCount;
			header.m_nIndex = i;
			header.m_nSize = fragmentSize;

			CBitOutStream fragmentStream;
			fragmentStream.Write (static_cast<uint8_t> (1));
			fragmentStream.Write (header);
			fragmentStream.WriteBytes (_rkOutStream.GetBytes (), i * FRAGMENT_SIZE, fragmentSize);
			m_pkUdpSession->Send (fragmentStream);
		}
	}
	else {
		m_pkUdpSession->Send (_rkOutStream);
	}

	// NOTE: in an extreme case sequence may overflow
	m_nOutSequence++;
}

template<typename TOutPacket>
int CUdpConnection<TOutPacket>::ResolveHeader (CBitInStream& _rkInStream)
{
	uint8_t isFragment;
	_rkInStream.Read (isFragment);

	if (isFragment == 1)
	{
		SFragmentHeader fragmentHeader;
		_rkInStream.Read (fragmentHeader);

		SFragmentReassembly* reassembly = m_kFragmentReassembly.TryGet (fragmentHeader.m_nSequence);
		if (reassembly == nullptr) {
			reassembly = &m_kFragmentReassembly.Insert (fragmentHeader.m_nSequence);
			reassembly->m_nCount = fragmentHeader.m_nCount;
			reassembly->m_nReceivedMask = 0;
			reassembly->m_nReceivedCount = 0;
		}

		if ((reassembly->m_nReceivedMask & (1 << fragmentHeader.m_nIndex)) != 0) {
			return 0;
		}

		reassembly->m_nReceivedMask |= 1 << fragmentHeader.m_nIndex;
		reassembly->m_nReceivedCount++;

		_rkInStream.ReadBytes (reassembly->m_kBytes, fragmentHeader.m_nIndex * FRAGMENT_SIZE, fragmentHeader.m_nSize);

		if (reassembly->m_nReceivedCount < fragmentHeader.m_nCount) {
			return 0;
		}

		_rkInStream = CBitInStream (reassembly->m_kBytes);
	}

	SUdpHeader header;
	_rkInStream.Read (header);

	int newInSequence = header.m_nSequence;
	int newOutAck = header.m_nAck;
	int newOutAckBits = header.m_nAckBits;

	if (newInSequence > m_nInSequence)
	{
		int distance = newInSequence - m_nInSequence;
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
		int distance = m_nInSequence - newInSequence;
		if (distance > 31) {
			return 0;
		}

		int ackBit = 1 << distance;
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

	int latestLostSequence = m_nOutAck >= 31 ? m_nOutAck - 31 : 0;
	int oldestLostSequence = newOutAck >= 31 ? newOutAck - 31 : 0;
	for (int sequence = oldestLostSequence; sequence < latestLostSequence; sequence++)
	{
		int distance = m_nOutAck >= sequence ? m_nOutAck - sequence : 0;
		int ackBit = 1 << distance;
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

	int latestSequence = m_nOutAck;
	int oldestSequence = m_nOutAck >= 31 ? m_nOutAck - 31 : 0;
	for (int sequence = oldestSequence; sequence <= latestSequence; sequence++)
	{
		int distance = m_nOutAck - sequence;
		int ackBit = 1 << distance;
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
}
