#include "stdafx.h"

#include "network/CNetProtocol.h"

#include "ServerEcho.h"

ServerEcho::ServerEcho ()
	: m_nNum (0)
{
}

ServerEcho::~ServerEcho ()
{
}

void ServerEcho::serialize (COutStream& _kOut_Stream)
{
}

void ServerEcho::deserialize (CInStream& _kIn_Stream)
{
	_kIn_Stream >> m_nNum;
}

void ServerEcho::excute ()
{
}
