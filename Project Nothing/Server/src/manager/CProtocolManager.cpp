#include "stdafx.h"

#include "protocol/ServerProtocols.h"
#include "protocol/ClientProtocols.h"
#include "CProtocolManager.h"

CProtocolManager::CProtocolManager ()
{
}

CProtocolManager::~CProtocolManager ()
{
}

void CProtocolManager::init ()
{
	register_protocol<ServerEcho> (100);
	register_protocol<ClientEcho> (200);
}

std::shared_ptr<INetProtocol> CProtocolManager::generate_protocol (const unsigned short _nProtocol_id, std::shared_ptr<CNetBridge>& _pNetBridge)
{
	std::shared_ptr<INetProtocol> pNet_protocol = m_kProtocol_map[_nProtocol_id]->generate ();
	pNet_protocol->set_net_bridge (_pNetBridge);
	return pNet_protocol;
}
