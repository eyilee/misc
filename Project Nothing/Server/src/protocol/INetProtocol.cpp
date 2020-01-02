#include "stdafx.h"

#include "network/CInStream.h"
#include "network/COutStream.h"
#include "INetProtocol.h"

INetProtocol::INetProtocol ()
	: m_nProtocol_id (0)
{
}

INetProtocol::~INetProtocol ()
{
}
