#include "stdafx.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/NetProtocolGenerator.h"
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

std::shared_ptr<INetProtocol> CProtocolManager::generate_protocol (unsigned short _nProtocolID)
{
	auto it = m_kProtocolMap.find (_nProtocolID);
	if (it == m_kProtocolMap.end ()) {
		return nullptr;
	}

	return m_kProtocolMap[_nProtocolID]->generate ();
}
