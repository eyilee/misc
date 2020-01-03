#include "stdafx.h"

#include "network/CInStream.h"
#include "network/COutStream.h"
#include "network/CNetBridge.h"

#include "INetProtocol.h"

unsigned short INetProtocol::m_nProtocol_id = 0;

INetProtocol::INetProtocol ()
	: m_pNet_Bridge (nullptr)
{
}

INetProtocol::~INetProtocol ()
{
}
