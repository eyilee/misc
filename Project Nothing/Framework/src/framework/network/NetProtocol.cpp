#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"

INetProtocol::INetProtocol ()
	: m_pkNetBridge (nullptr)
{
}

INetProtocol::~INetProtocol ()
{
}
