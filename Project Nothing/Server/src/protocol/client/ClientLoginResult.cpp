#include "stdafx.h"
#include "protocol/client/ClientLoginResult.h"

ClientLoginResult::ClientLoginResult ()
{
}

ClientLoginResult::ClientLoginResult (const int& _rnID, const uint32_t& _rnKey)
	: m_nID (_rnID)
	, m_nKey (_rnKey)
{
}

ClientLoginResult::~ClientLoginResult ()
{
}

void ClientLoginResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nID);
	_rkOutStream.Write (m_nKey);
}

void ClientLoginResult::Deserialize (CBitInStream& _rkInStream)
{
}

void ClientLoginResult::Excute ()
{
}
