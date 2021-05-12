#include "stdafx.h"

#include "framework/network/Entity.h"

#include "framework/manager/BaseManager.h"
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
