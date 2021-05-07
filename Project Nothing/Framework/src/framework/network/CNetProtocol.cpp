#include "stdafx.h"

#include "framework/network/CInStream.h"
#include "framework/network/CNetBridge.h"
#include "framework/network/COutStream.h"
#include "framework/network/CNetProtocol.h"

INetProtocol::INetProtocol ()
	: m_pNet_Bridge (nullptr)
{
}

INetProtocol::~INetProtocol ()
{
}
