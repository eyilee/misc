#include "stdafx.h"
#include "protocol/netcommand/NC_ClientCreateGameResult.h"

NC_ClientCreateGameResult::NC_ClientCreateGameResult ()
{
}

NC_ClientCreateGameResult::NC_ClientCreateGameResult (uint32_t _nGameID)
	: m_nGameID (_nGameID)
{
}

NC_ClientCreateGameResult::~NC_ClientCreateGameResult ()
{
}

void NC_ClientCreateGameResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nGameID);
}
