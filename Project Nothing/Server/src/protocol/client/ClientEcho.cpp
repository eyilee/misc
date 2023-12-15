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

void ClientEcho::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_kString);
}

void ClientEcho::Deserialize (CBitInStream& _rkInStream)
{
}

void ClientEcho::Excute ()
{
}
