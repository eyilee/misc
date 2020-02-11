#include "stdafx.h"

#include "CEntityManager.h"

CEntityManager::CEntityManager ()
{
}

CEntityManager::~CEntityManager ()
{
}

void CEntityManager::init ()
{
}

std::shared_ptr<CEntity> CEntityManager::create_entity ()
{
	return std::shared_ptr<CEntity> ();
}

