#pragma once

class NE_ServerShutdown : public INetEvent<NE_ServerShutdown>
{
public:
	NE_ServerShutdown ();
	virtual ~NE_ServerShutdown ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};
