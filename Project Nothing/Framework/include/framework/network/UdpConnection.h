#pragma once
#include "framework/network/BitStream.h"
#include "framework/network/SequenceBuffer.h"

using boost::asio::ip::udp;

class INetProtocol;
class CUdpSession;

constexpr uint32_t SEQUENCE_BUFFER_SIZE = 256;

class CUdpConnection : public std::enable_shared_from_this<CUdpConnection>
{
protected:
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

	enum class EPackageType
	{
		None = 0,
	};

	struct SOutPacket
	{
		EPackageType m_nPackageType;
		bool m_bReliable;
		std::shared_ptr<INetProtocol> m_pkProtocol;

		SOutPacket ()
		{
			Reset ();
		}

		void Reset ()
		{
			m_nPackageType = EPackageType::None;
			m_bReliable = false;
			m_pkProtocol = nullptr;
		}
	};

public:
	CUdpConnection (std::shared_ptr<CUdpSession> _pkUdpSession);
	virtual ~CUdpConnection ();

	inline udp::endpoint GetLocalEndpoint () const { return m_kLocalEndPoint; }

	void Init ();
	void Shutdown ();
	void OnDisconnect ();

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (EPackageType _nPackageType, bool _bReliable, std::shared_ptr<INetProtocol> _pkProtocol);

protected:
	uint32_t ResolveHeader (CBitInStream& _rkInStream);
	virtual void ResolvePackage (CBitInStream& _rkInStream);
	virtual void OnPacketAcked (uint32_t _nSequence, SOutPacket& _rkOutPacket);

protected:
	std::shared_ptr<CUdpSession> m_pkUdpSession;
	udp::endpoint m_kLocalEndPoint;

	uint32_t m_nKey;
	uint32_t m_nInSequence;
	uint32_t m_nInAckBits;
	uint32_t m_nOutSequence;
	uint32_t m_nOutAck;
	uint32_t m_nOutAckBits;
	SequenceBuffer<SOutPacket, SEQUENCE_BUFFER_SIZE> m_kOutPackets;
};