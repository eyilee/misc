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

void CProtocolManager::Init ()
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CProtocolManager> ();
}

void CProtocolManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance = nullptr;
}

std::shared_ptr<INetProtocol> CProtocolManager::GenerateProtocol (unsigned short _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->Generate (_nID);
}

std::shared_ptr<INetProtocol> CProtocolManager::Generate (unsigned short _nID)
{
	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		return nullptr;
	}

	return m_kProtocolMap[_nID]->Generate ();
}
