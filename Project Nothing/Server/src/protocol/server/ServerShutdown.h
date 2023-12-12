#pragma once

class ServerShutdown : public CNetProtocol<ServerShutdown>
{
public:
	ServerShutdown ();
	virtual ~ServerShutdown ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};