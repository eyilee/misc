#pragma once

class NE_ServerShutdown : public CNetProtocol<NE_ServerShutdown>
{
public:
	NE_ServerShutdown ();
	virtual ~NE_ServerShutdown ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};