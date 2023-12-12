#pragma once

class IEntity;
class INetProtocol;
class CBitInStream;
class CTcpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge (std::shared_ptr<CTcpSession> _pkSession, const uint32_t& _rnIP);
	virtual ~CNetBridge ();

	uint32_t GetIP () { return m_nIP; }

	void SetKey (const uint32_t& _rnKey) { m_nKey = _rnKey; }
	uint32_t GetKey () { return m_nKey; }

	void SetEntity (std::shared_ptr<IEntity> _pkEntity) { m_pkEntity = _pkEntity; }
	std::shared_ptr<IEntity> GetEntity () { return m_pkEntity; }

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	std::shared_ptr<CTcpSession> m_pkSession;
	uint32_t m_nIP;
	uint32_t m_nKey;
	std::shared_ptr<IEntity> m_pkEntity;
};