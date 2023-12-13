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

	void SetTcpSession (std::shared_ptr<CTcpSession> _pkTcpSession) { m_pkTcpSession = _pkTcpSession; }

	void SetUdpEndPoint (const short& _nPort);
	uint32_t GetUdpIP () const { return m_nUdpIP; }
	uint32_t GetUdpKey () const { return m_nUdpKey; }

	void SetEntity (std::shared_ptr<IEntity> _pkEntity) { m_pkEntity = _pkEntity; }
	std::shared_ptr<IEntity> GetEntity () { return m_pkEntity; }

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol);
	void UdpOutput (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	std::shared_ptr<CTcpSession> m_pkTcpSession;

	udp::endpoint m_kUdpEndPoint;
	uint32_t m_nUdpIP;
	uint32_t m_nUdpKey;

	std::shared_ptr<IEntity> m_pkEntity;
};