#include "stdafx.h"
#include "protocol/netcommand/NC_ClientJoinGame.h"

NC_ClientJoinGame::NC_ClientJoinGame ()
{
}

NC_ClientJoinGame::NC_ClientJoinGame (uint32_t _nGameID)
	: m_nGameID (_nGameID)
{
}

NC_ClientJoinGame::~NC_ClientJoinGame ()
{
}

void NC_ClientJoinGame::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nGameID);
}
