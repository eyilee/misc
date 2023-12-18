#pragma once

class NE_ServerShutdown : public CNetEvent<NE_ServerShutdown>
{
public:
	NE_ServerShutdown ();
	virtual ~NE_ServerShutdown ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};