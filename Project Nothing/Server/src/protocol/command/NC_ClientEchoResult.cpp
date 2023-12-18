#include "stdafx.h"
#include "protocol/command/NC_ClientEchoResult.h"

NC_ClientEchoResult::NC_ClientEchoResult ()
{
}

NC_ClientEchoResult::NC_ClientEchoResult (const std::wstring& _rkString)
	: m_kString (_rkString)
{
}

NC_ClientEchoResult::~NC_ClientEchoResult ()
{
}

void NC_ClientEchoResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_kString);
}

void NC_ClientEchoResult::Deserialize (CBitInStream& _rkInStream)
{
}

void NC_ClientEchoResult::Excute ()
{
}
