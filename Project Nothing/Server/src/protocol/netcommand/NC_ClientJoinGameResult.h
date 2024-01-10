#pragma once

class NC_ClientJoinGameResult : public INetCommand<NC_ClientJoinGameResult>
{
public:
	NC_ClientJoinGameResult ();
	NC_ClientJoinGameResult (uint32_t _nGameID, unsigned short _nPort);
	virtual ~NC_ClientJoinGameResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	uint32_t m_nGameID;
	unsigned short m_nPort;
};