#include "stdafx.h"
#include "framework/network/NetEntity.h"
#include "framework/manager/NetEntityManager.h"

CNetEntityManager::CNetEntityManager ()
{
}

CNetEntityManager::~CNetEntityManager ()
{
}

void CNetEntityManager::Init ()
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CNetEntityManager> ();
}

void CNetEntityManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance = nullptr;
}

std::shared_ptr<INetEntity> CNetEntityManager::GetNetEntity (int _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->Get (_nID);
}

std::shared_ptr<INetEntity> CNetEntityManager::Get (int _nID)
{
	auto it = m_kNetEntityMap.find (_nID);
	if (it == m_kNetEntityMap.end ())
	{
		return nullptr;
	}

	return it->second;
}
