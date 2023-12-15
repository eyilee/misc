#pragma once

using boost::asio::ip::udp;

class IEntity;
class INetProtocol;
class CBitInStream;
class CTcpSession;
class CUdpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge ();
	virtual ~CNetBridge ();

	void Init (std::shared_ptr<CTcpSession> _pkTcpSession, const uint32_t& _rnIP);
	void Shutdown ();

	uint32_t GetIP () const { return m_nIP; }

	void SetUdpKey (const uint32_t& _rnUdpKey) { m_nUdpKey = _rnUdpKey; }
	uint32_t GetUdpKey () const { return m_nUdpKey; }

	void SetUdpEndPoint ();

	void SetEntity (std::shared_ptr<IEntity> _pkEntity) { m_pkEntity = _pkEntity; }
	std::shared_ptr<IEntity> GetEntity () const { return m_pkEntity; }

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeTcpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const;
	void ComposeUdpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const;

private:
	std::shared_ptr<CTcpSession> m_pkTcpSession;
	uint32_t m_nIP;

	uint32_t m_nUdpKey;
	udp::endpoint m_kUdpEndPoint;

	std::shared_ptr<IEntity> m_pkEntity;
};