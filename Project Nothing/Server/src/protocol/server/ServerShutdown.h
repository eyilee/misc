#pragma once

class ServerShutdown : public CNetProtocol<ServerShutdown>
{
public:
	ServerShutdown ();
	virtual ~ServerShutdown ();

	virtual void serialize (CBitOutStream& _rkOutStream) override;
	virtual void deserialize (CBitInStream& _rkInStream) override;
	virtual void excute () override;
};