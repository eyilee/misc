#include "stdafx.h"

#include "framework/network/InStream.h"
#include "framework/network/NetBridge.h"
#include "framework/network/OutStream.h"
#include "framework/network/NetProtocol.h"

INetProtocol::INetProtocol ()
	: m_pNet_Bridge (nullptr)
{
}

INetProtocol::~INetProtocol ()
{
}
