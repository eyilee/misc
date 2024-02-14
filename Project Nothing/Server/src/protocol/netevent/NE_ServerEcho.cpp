#include "stdafx.h"
#include "protocol/netcommand/NC_ClientEchoResult.h"
#include "protocol/netevent/NE_ServerEcho.h"

NE_ServerEcho::NE_ServerEcho ()
	: m_nSequence (0)
	, m_nSendTime (0)
{
}

NE_ServerEcho::~NE_ServerEcho ()
{
}

void NE_ServerEcho::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nSequence);
	_rkInStream.Read (m_nSendTime);
}

void NE_ServerEcho::Excute ()
{
	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientEchoResult> (m_nSequence, m_nSendTime);
	m_pkNetBridge->ComposeOutput (protocol);
}
