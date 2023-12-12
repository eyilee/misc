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
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}
}

void CEntityManager::Shutdown ()
{
	m_kEntityMap.clear ();

	Instance = nullptr;
}

std::shared_ptr<IEntity> CEntityManager::GetEntity (int _nID)
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		return nullptr;
	}

	return it->second;
}
