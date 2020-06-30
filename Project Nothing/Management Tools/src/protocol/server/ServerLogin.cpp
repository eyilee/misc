#include "stdafx.h"

#include "protocol/server/ServerLogin.h"

ServerLogin::ServerLogin ()
{
}

ServerLogin::ServerLogin (const int _nId)
	: m_nId (_nId)
{
}

ServerLogin::~ServerLogin ()
{
}

void ServerLogin::serialize (COutStream& _kOut_Stream)
{
	_kOut_Stream << m_nId;
}

void ServerLogin::deserialize (CInStream& _kIn_Stream)
{
}

void ServerLogin::excute ()
{
}
