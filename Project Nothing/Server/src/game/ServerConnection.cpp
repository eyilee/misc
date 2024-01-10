#include "stdafx.h"
#include "game/ServerConnection.h"

CServerConnection::CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession)
	: CUdpConnection (_pkUdpSession)
{
}

CServerConnection::~CServerConnection ()
{
}

void CServerConnection::ResolvePackage (CBitInStream& _rkInStream)
{
}

void CServerConnection::OnPacketAcked (uint32_t _nSequence, SOutPacket& _rkOutPacket)
{
}
