#pragma once

class ServerLogin : public CNetProtocol<ServerLogin>
{
public:
	ServerLogin ();
	virtual ~ServerLogin ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	int m_nID;
};