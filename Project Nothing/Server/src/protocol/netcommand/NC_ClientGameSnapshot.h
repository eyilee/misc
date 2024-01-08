#pragma once

class NC_ClientGameSnapshot : public INetCommand<NC_ClientGameSnapshot>
{
public:
	NC_ClientGameSnapshot ();
	virtual ~NC_ClientGameSnapshot ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
};