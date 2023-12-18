#pragma once

class NE_ServerLogin : public CNetProtocol<NE_ServerLogin>
{
public:
	NE_ServerLogin ();
	virtual ~NE_ServerLogin ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	int m_nID;
};