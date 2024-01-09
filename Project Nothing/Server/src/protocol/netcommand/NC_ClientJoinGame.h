#pragma once

class NC_ClientJoinGame : public INetCommand<NC_ClientJoinGame>
{
public:
	NC_ClientJoinGame ();
	NC_ClientJoinGame (uint32_t _nGameID);
	virtual ~NC_ClientJoinGame ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	uint32_t m_nGameID;
};