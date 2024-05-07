#pragma once

class NE_ServerLogin : public INetEvent<NE_ServerLogin>
{
public:
	NE_ServerLogin ();
	virtual ~NE_ServerLogin ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	uint32_t m_nID;
};
