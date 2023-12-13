#include "stdafx.h"
#include "framework/manager/EntityManager.h"

CEntityManager::CEntityManager ()
{
}

CEntityManager::~CEntityManager ()
{
}

void CEntityManager::Init ()
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CEntityManager> ();
}

void CEntityManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance = nullptr;
}

std::shared_ptr<IEntity> CEntityManager::GetEntity (int _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->Get (_nID);
}

std::shared_ptr<IEntity> CEntityManager::Get (int _nID)
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		return nullptr;
	}

	return it->second;
}
