#pragma once

class ServerLogin : public CNetProtocol<ServerLogin>
{
public:
	ServerLogin ();
	virtual ~ServerLogin ();

	virtual void serialize (COutStream& _kOut_stream) override;
	virtual void deserialize (CInStream& _kIn_stream) override;
	virtual void excute () override;

private:
	int m_nId;
};