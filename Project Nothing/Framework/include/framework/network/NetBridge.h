#pragma once

class IEntity;
class CInStream;
class INetProtocol;
class CTcpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge (std::shared_ptr<CTcpSession> _pkSession);
	virtual ~CNetBridge ();

	void set_entity (std::shared_ptr<IEntity> _pkEntity);
	std::shared_ptr<IEntity> get_entity ();

	void resolve_input (CInStream& _rkInStream);
	void compose_output (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	std::shared_ptr<CTcpSession> m_pkSession;
	std::shared_ptr<IEntity> m_pkEntity;
};