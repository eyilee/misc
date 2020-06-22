#pragma once

class CEntity;
class CInStream;
class INetProtocol;
class CTcpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge (std::shared_ptr<CTcpSession>& _pSession);
	virtual ~CNetBridge ();

	void set_entity (std::shared_ptr<CEntity>& _pEntity);
	std::shared_ptr<CEntity>& get_entity ();

	void resolve_input (CInStream& _kIn_Stream);
	void compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol);

private:
	std::shared_ptr<INetProtocol> generate_protocol (unsigned short _nProtocol_id, std::shared_ptr<CNetBridge>& _pNetBridge);

private:
	std::shared_ptr<CTcpSession> m_pSession;
	std::shared_ptr<CEntity> m_pEntity;
};