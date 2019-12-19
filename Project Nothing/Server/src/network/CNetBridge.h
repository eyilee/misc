#pragma once

class CEntity;
class CTcpSession;

class CNetBridge
{
public:
	CNetBridge (const std::shared_ptr<CTcpSession>& _pSession);
	virtual ~CNetBridge ();

	void set_entity (std::shared_ptr<CEntity>& _pEntity);
	std::shared_ptr<CEntity>& get_entity ();

private:
	std::shared_ptr<CTcpSession> m_pSession;
	std::shared_ptr<CEntity> m_pEntity;
};