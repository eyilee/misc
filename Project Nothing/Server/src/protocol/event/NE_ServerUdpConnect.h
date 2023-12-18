#pragma once

class NE_ServerUdpConnect : public CNetProtocol<NE_ServerUdpConnect>
{
public:
	NE_ServerUdpConnect ();
	virtual ~NE_ServerUdpConnect ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};