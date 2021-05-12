#include "stdafx.h"

#include "framework/network/NetProtocol.h"
#include "framework/network/NetProtocolGenerator.h"

#include "framework/manager/BaseManager.h"
#include "framework/manager/ProtocolManager.h"

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
