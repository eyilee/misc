#pragma once

class ServerShutdown : public CNetProtocol<ServerShutdown>
{
public:
	ServerShutdown ();
	virtual ~ServerShutdown ();

	virtual void serialize (COutStream& _rkOutStream) override;
	virtual void deserialize (CInStream& _rkInStream) override;
	virtual void excute () override;
};