#include "stdafx.h"
#include "framework/network/Entity.h"
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

void CEntityManager::set_entity (int _nID, std::shared_ptr<IEntity> _pkEntity)
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		m_kEntityMap.emplace (_nID, _pkEntity);
	}
	else {
		it->second = _pkEntity;
	}
}

void CEntityManager::remove_entity (int _nID)
{
	m_kEntityMap.erase (_nID);
}
