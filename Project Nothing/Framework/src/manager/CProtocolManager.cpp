#include "stdafx.h"

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

void CProtocolManager::shutdown ()
{
	Instance = nullptr;
}

std::shared_ptr<INetProtocol> CProtocolManager::generate_protocol (const unsigned short _nProtocol_id)
{
	return m_kProtocol_map[_nProtocol_id]->generate ();
}
