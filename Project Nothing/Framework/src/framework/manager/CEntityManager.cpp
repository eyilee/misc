#include "stdafx.h"

#include "framework/network/CEntity.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CEntityManager.h"

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
