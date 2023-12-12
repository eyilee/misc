#include "stdafx.h"
#include "framework/manager/EntityManager.h"

CEntityManager::CEntityManager ()
{
}

CEntityManager::~CEntityManager ()
{
}

void CEntityManager::init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}
}

void CEntityManager::shutdown ()
{
	Instance = nullptr;
}

std::shared_ptr<IEntity> CEntityManager::get_entity (int _nID)
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		return nullptr;
	}

	return it->second;
}
