#pragma once

class CInStream;
class CEntity;
class CTcpSession;

class CNetBridge
{
public:
	CNetBridge (std::shared_ptr<CTcpSession>& _pSession);
	virtual ~CNetBridge ();

	void set_entity (std::shared_ptr<CEntity>& _pEntity);
	std::shared_ptr<CEntity>& get_entity ();

	void resolve_input (CInStream& _kIn_Stream);

private:
	std::shared_ptr<CTcpSession> m_pSession;
	std::shared_ptr<CEntity> m_pEntity;
};