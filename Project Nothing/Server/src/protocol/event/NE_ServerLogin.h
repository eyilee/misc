#pragma once

class NE_ServerLogin : public CNetEvent<NE_ServerLogin>
{
public:
	NE_ServerLogin ();
	virtual ~NE_ServerLogin ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	int m_nID;
};