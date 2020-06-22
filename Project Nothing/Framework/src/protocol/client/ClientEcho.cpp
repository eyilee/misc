#include "stdafx.h"

#include "ClientEcho.h"

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

void ClientEcho::serialize (COutStream& _kOut_Stream)
{
	_kOut_Stream << m_kString;
}

void ClientEcho::deserialize (CInStream& _kIn_Stream)
{
}

void ClientEcho::excute ()
{
}
