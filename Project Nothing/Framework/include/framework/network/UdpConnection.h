#pragma once
#include "framework/network/BitStream.h"
#include "framework/network/SequenceBuffer.h"

using boost::asio::ip::udp;

class INetProtocol;
class CNetBridge;
class CUdpSession;

constexpr uint32_t SEQUENCE_BUFFER_SIZE = 256;

class CUdpConnection : public std::enable_shared_from_this<CUdpConnection>
{
private:
	struct SHeader : public IBitSerializable
	{
		uint32_t m_nSequence;
		uint32_t m_nAck;
		uint32_t m_nAckBits;

		SHeader ();
		virtual ~SHeader ();

		virtual void Serialize (CBitOutStream& _rkOutStream) override;
		virtual void Deserialize (CBitInStream& _rkInStream) override;
	};

	struct SInPacket
	{
		bool m_bAcked;
	};

	struct SOutPacket
	{
		bool m_bReliable;
		std::vector<uint8_t> m_kBytes;
	};

public:
	CUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CUdpSession> _pkUdpSession);
	virtual ~CUdpConnection ();

	void Init ();
	void Shutdown ();

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	void OnPacketAcked (uint32_t _nSequence, SOutPacket* _pkOutPacket);

private:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
	std::shared_ptr<CUdpSession> m_pkUdpSession;
	udp::endpoint m_kEndPoint;

	uint32_t m_nInSequece;
	uint32_t m_nInAckBits;
	SequenceBuffer<SInPacket, SEQUENCE_BUFFER_SIZE> m_kInPackets;

	uint32_t m_nOutSequece;
	uint32_t m_nOutAck;
	uint32_t m_nOutAckBits;
	SequenceBuffer<SOutPacket, SEQUENCE_BUFFER_SIZE> m_kOutPackets;
};