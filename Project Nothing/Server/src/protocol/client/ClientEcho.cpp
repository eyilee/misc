#include "stdafx.h"
#include "protocol/client/ClientEcho.h"

ClientEcho::ClientEcho ()
{
}

ClientEcho::ClientEcho (const std::wstring& _rkString)
	: m_kString (_rkString)
{
}

ClientEcho::~ClientEcho ()
{
}

void ClientEcho::serialize (COutStream& _rkOutStream)
{
	_rkOutStream << m_kString;
}

void ClientEcho::deserialize (CInStream& _rkInStream)
{
}

void ClientEcho::excute ()
{
}
