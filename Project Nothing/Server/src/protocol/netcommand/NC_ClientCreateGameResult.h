#pragma once

class NC_ClientCreateGameResult : public INetCommand<NC_ClientCreateGameResult>
{
public:
	NC_ClientCreateGameResult ();
	virtual ~NC_ClientCreateGameResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
};