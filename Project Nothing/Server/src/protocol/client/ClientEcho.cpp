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

	for (size_t i = 0; i < 4000; i++) {
		_rkOutStream.Write (0);
	}
}

void ClientEcho::Deserialize (CBitInStream& _rkInStream)
{
}

void ClientEcho::Excute ()
{
}
