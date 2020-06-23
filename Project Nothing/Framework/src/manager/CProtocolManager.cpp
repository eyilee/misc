#include "stdafx.h"

#include "framework/network/CNetBridge.h"
#include "framework/network/CNetProtocol.h"
#include "framework/network/CNetProtocolGenerator.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CProtocolManager.h"

CProtocolManager::CProtocolManager ()
{
}

CProtocolManager::~CProtocolManager ()
{
}

void CProtocolManager::init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}
}

std::shared_ptr<INetProtocol> CProtocolManager::generate_protocol (const unsigned short _nProtocol_id, std::shared_ptr<CNetBridge>& _pNetBridge)
{
	std::shared_ptr<INetProtocol> pNet_protocol = m_kProtocol_map[_nProtocol_id]->generate ();
	pNet_protocol->set_net_bridge (_pNetBridge);
	return pNet_protocol;
}
