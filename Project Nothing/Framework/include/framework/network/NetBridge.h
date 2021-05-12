#pragma once

class IEntity;
class CInStream;
class INetProtocol;
class CTcpSession;

class CNetBridge : public std::enable_shared_from_this<CNetBridge>
{
public:
	CNetBridge (std::shared_ptr<CTcpSession>&& _pSession);
	virtual ~CNetBridge ();

	void set_entity (std::shared_ptr<IEntity>&& _pEntity);
	std::shared_ptr<IEntity> get_entity ();

	void resolve_input (CInStream& _kIn_Stream);
	void compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol);

private:
	std::shared_ptr<CTcpSession> m_pSession;
	std::shared_ptr<IEntity> m_pEntity;
};