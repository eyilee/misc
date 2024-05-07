#include "stdafx.h"
#include "protocol/netcommand/NC_ClientJoinGameResult.h"

NC_ClientJoinGameResult::NC_ClientJoinGameResult ()
	: m_nGameID (0)
	, m_nPort (0)
	, m_nKey (0)
{
}

NC_ClientJoinGameResult::NC_ClientJoinGameResult (uint32_t _nGameID, unsigned short _nPort, uint32_t _nKey)
	: m_nGameID (_nGameID)
	, m_nPort (_nPort)
	, m_nKey (_nKey)
{
}

NC_ClientJoinGameResult::~NC_ClientJoinGameResult ()
{
}

void NC_ClientJoinGameResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nGameID);
	_rkOutStream.Write (m_nPort);
	_rkOutStream.Write (m_nKey);
}
