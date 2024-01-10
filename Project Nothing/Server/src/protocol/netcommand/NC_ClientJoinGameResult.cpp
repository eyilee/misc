#include "stdafx.h"
#include "protocol/netcommand/NC_ClientJoinGameResult.h"

NC_ClientJoinGameResult::NC_ClientJoinGameResult ()
{
}

NC_ClientJoinGameResult::NC_ClientJoinGameResult (uint32_t _nGameID, unsigned short _nPort)
	: m_nGameID (_nGameID)
	, m_nPort (_nPort)
{
}

NC_ClientJoinGameResult::~NC_ClientJoinGameResult ()
{
}

void NC_ClientJoinGameResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nGameID);
	_rkOutStream.Write (m_nPort);
}
