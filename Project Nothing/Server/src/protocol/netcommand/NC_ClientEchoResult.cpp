#include "stdafx.h"
#include "protocol/netcommand/NC_ClientEchoResult.h"

NC_ClientEchoResult::NC_ClientEchoResult ()
	: m_nSequence (0)
	, m_nSendTime (0)
{
}

NC_ClientEchoResult::NC_ClientEchoResult (unsigned short _nSequence, long long _nSendTime)
	: m_nSequence (_nSequence)
	, m_nSendTime (_nSendTime)
{
}

NC_ClientEchoResult::~NC_ClientEchoResult ()
{
}

void NC_ClientEchoResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nSequence);
	_rkOutStream.Write (m_nSendTime);
}
