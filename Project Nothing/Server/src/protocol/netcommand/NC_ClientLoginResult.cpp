#include "stdafx.h"
#include "protocol/netcommand/NC_ClientLoginResult.h"

NC_ClientLoginResult::NC_ClientLoginResult ()
	: m_nID (0)
{
}

NC_ClientLoginResult::NC_ClientLoginResult (uint32_t _nID)
	: m_nID (_nID)
{
}

NC_ClientLoginResult::~NC_ClientLoginResult ()
{
}

void NC_ClientLoginResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nID);
}
