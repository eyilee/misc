#pragma once

class CInStream;
class COutStream;
class CEntity;
class CTcpSession;
class INetProtocol;

class CNetBridge
{
public:
	CNetBridge (std::shared_ptr<CTcpSession>& _pSession);
	virtual ~CNetBridge ();

	void set_entity (std::shared_ptr<CEntity>& _pEntity);
	std::shared_ptr<CEntity>& get_entity ();

	void resolve_input (CInStream& _kIn_Stream);
	void compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol);

private:
	std::shared_ptr<INetProtocol> generate_protocol (unsigned short _nProtocol_id);

private:
	std::shared_ptr<CTcpSession> m_pSession;
	std::shared_ptr<CEntity> m_pEntity;
};