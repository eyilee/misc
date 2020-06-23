#include "stdafx.h"

#include "network/CEntity.h"

#include "manager/CEntityManager.h"

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

std::shared_ptr<CEntity> CEntityManager::create_entity ()
{
	return std::shared_ptr<CEntity> ();
}

