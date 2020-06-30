#pragma once

class ServerShutdown : public CNetProtocol<ServerShutdown>
{
public:
	ServerShutdown ();
	virtual ~ServerShutdown ();

	virtual void serialize (COutStream& _kOut_Stream) override;
	virtual void deserialize (CInStream& _kIn_Stream) override;
	virtual void excute () override;
};