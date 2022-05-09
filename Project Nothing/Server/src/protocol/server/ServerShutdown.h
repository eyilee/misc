#pragma once

class ServerShutdown : public CNetProtocol<ServerShutdown>
{
public:
	ServerShutdown ();
	virtual ~ServerShutdown ();

	virtual void serialize (COutStream& _kOut_stream) override;
	virtual void deserialize (CInStream& _kIn_stream) override;
	virtual void excute () override;
};