#pragma once

class NE_ServerUdpConnect : public CNetEvent<NE_ServerUdpConnect>
{
public:
	NE_ServerUdpConnect ();
	virtual ~NE_ServerUdpConnect ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	unsigned short m_nPort;
};