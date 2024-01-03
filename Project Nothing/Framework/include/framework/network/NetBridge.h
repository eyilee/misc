#pragma once

using boost::asio::ip::udp;

class IEntity;
class INetProtocol;
class CBitInStream;
class CTcpConnection;
class CTcpSession;
class CUdpConnection;
class CUdpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge ();
	virtual ~CNetBridge ();

	void SetTcpConnection (std::shared_ptr<CTcpConnection> _pkTcpConnection);
	inline void SetUdpConnection (std::shared_ptr<CUdpConnection> _pkUdpConnection) { m_pkUdpConnection = _pkUdpConnection; }

	void Shutdown ();

	uint32_t GetIP ();

	uint32_t GetKey () const { return m_nKey; }
	void SetKey (uint32_t _nKey) { m_nKey = _nKey; }

	void SetEntity (std::shared_ptr<IEntity> _pkEntity) { m_pkEntity = _pkEntity; }
	std::shared_ptr<IEntity> GetEntity () const { return m_pkEntity; }

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeTcpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const;
	void ComposeUdpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const;

private:
	uint32_t m_nIP;
	uint32_t m_nKey;

	std::shared_ptr<CTcpConnection> m_pkTcpConnection;
	std::shared_ptr<CUdpConnection> m_pkUdpConnection;
	std::shared_ptr<IEntity> m_pkEntity;
};