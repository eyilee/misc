#include "stdafx.h"

#include "network/CNetBridge.h"
#include "INetProtocol.h"
#include "INetProtocolGenerator.h"
#include "CNetProtocolGenerator.h"

#include "CProtocolManager.h"

CProtocolManager::CProtocolManager ()
{
}

CProtocolManager::~CProtocolManager ()
{
}

void CProtocolManager::init ()
{
}

std::shared_ptr<INetProtocol> CProtocolManager::generate_protocol (const unsigned short _nProtocol_id, std::shared_ptr<CNetBridge>& _pNetBridge)
{
	std::shared_ptr<INetProtocol> pNet_protocol = m_kProtocol_map[_nProtocol_id]->generate ();
	pNet_protocol->set_net_bridge (_pNetBridge);
	return pNet_protocol;
}
