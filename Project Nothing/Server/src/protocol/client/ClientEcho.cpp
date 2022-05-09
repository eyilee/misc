#include "stdafx.h"

#include "protocol/client/ClientEcho.h"

ClientEcho::ClientEcho ()
{
}

ClientEcho::ClientEcho (const std::string& _kString)
	: m_kString (_kString)
{
}

ClientEcho::~ClientEcho ()
{
}

void ClientEcho::serialize (COutStream& _kOut_stream)
{
	_kOut_stream << m_kString;
}

void ClientEcho::deserialize (CInStream& _kIn_stream)
{
}

void ClientEcho::excute ()
{
}
