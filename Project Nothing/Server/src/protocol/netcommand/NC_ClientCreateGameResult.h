#pragma once

class NC_ClientCreateGameResult : public INetCommand<NC_ClientCreateGameResult>
{
public:
	NC_ClientCreateGameResult ();
	NC_ClientCreateGameResult (uint32_t _nGameID);
	virtual ~NC_ClientCreateGameResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	uint32_t m_nGameID;
};
