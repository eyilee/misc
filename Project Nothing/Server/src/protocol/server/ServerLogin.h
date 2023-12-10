#pragma once

class ServerLogin : public CNetProtocol<ServerLogin>
{
public:
	ServerLogin ();
	virtual ~ServerLogin ();

	virtual void serialize (COutStream& _rkOutStream) override;
	virtual void deserialize (CInStream& _rkInStream) override;
	virtual void excute () override;

private:
	int m_nID;
};