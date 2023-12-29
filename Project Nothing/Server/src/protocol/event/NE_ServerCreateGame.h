#pragma once

class NE_ServerCreateGame : public CNetEvent<NE_ServerCreateGame>
{
public:
	NE_ServerCreateGame ();
	virtual ~NE_ServerCreateGame ();

	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};