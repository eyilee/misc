#include "stdafx.h"
#include "protocol/command/NC_ClientLoginResult.h"

NC_ClientLoginResult::NC_ClientLoginResult ()
{
}

NC_ClientLoginResult::NC_ClientLoginResult (int _nID, uint32_t _nKey)
	: m_nID (_nID)
	, m_nKey (_nKey)
{
}

NC_ClientLoginResult::~NC_ClientLoginResult ()
{
}

void NC_ClientLoginResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nID);
	_rkOutStream.Write (m_nKey);
}
