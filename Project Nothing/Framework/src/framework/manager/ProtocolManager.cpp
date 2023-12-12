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
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}
}
									   
void CProtocolManager::Shutdown ()
{
	Instance = nullptr;
}

std::shared_ptr<INetProtocol> CProtocolManager::GenerateProtocol (unsigned short _nID)
{
	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		return nullptr;
	}

	return m_kProtocolMap[_nID]->Generate ();
}
