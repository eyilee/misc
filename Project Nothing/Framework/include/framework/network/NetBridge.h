#pragma once

class IEntity;
class CBitInStream;
class INetProtocol;
class CTcpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge (std::shared_ptr<CTcpSession> _pkSession, const uint32_t& _rnIP);
	virtual ~CNetBridge ();

	uint32_t get_ip () { return m_nIP; }

	void set_key (const uint32_t& _rnKey) { m_nKey = _rnKey; }
	uint32_t get_key () { return m_nKey; }

	void set_entity (std::shared_ptr<IEntity> _pkEntity) { m_pkEntity = _pkEntity; }
	std::shared_ptr<IEntity> get_entity () { return m_pkEntity; }

	void resolve_input (CBitInStream& _rkInStream);
	void compose_output (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	std::shared_ptr<CTcpSession> m_pkSession;
	uint32_t m_nIP;
	uint32_t m_nKey;
	std::shared_ptr<IEntity> m_pkEntity;
};