#include "stdafx.h"

#include "network/CInStream.h"
#include "network/CNetBridge.h"
#include "network/CNetProtocol.h"

INetProtocol::INetProtocol ()
	: m_pNet_Bridge (nullptr)
{
}

INetProtocol::~INetProtocol ()
{
}
