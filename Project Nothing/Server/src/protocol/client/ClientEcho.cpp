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

void ClientEcho::serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_kString);
}

void ClientEcho::deserialize (CBitInStream& _rkInStream)
{
}

void ClientEcho::excute ()
{
}
