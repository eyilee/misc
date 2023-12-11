#pragma once

class ServerLogin : public CNetProtocol<ServerLogin>
{
public:
	ServerLogin ();
	virtual ~ServerLogin ();

	virtual void serialize (CBitOutStream& _rkOutStream) override;
	virtual void deserialize (CBitInStream& _rkInStream) override;
	virtual void excute () override;

private:
	int m_nID;
};