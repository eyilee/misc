#include "stdafx.h"
#include "PlayerEntity.h"
#include "protocol/server/ServerLogin.h"

ServerLogin::ServerLogin ()
	: m_nID (0)
{
}

ServerLogin::~ServerLogin ()
{
}

void ServerLogin::serialize (CBitOutStream& _rkOutStream)
{
}

void ServerLogin::deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nID);
}

void ServerLogin::excute ()
{
	if (CEntityManager::Instance != nullptr)
	{
		std::shared_ptr<CPlayerEntity> entity = CEntityManager::Instance->create_entity<CPlayerEntity> (m_nID);

		entity->set_id (m_nID);

		if (m_pkNetBridge != nullptr)
		{
			if (m_pkNetBridge->get_entity () == nullptr) {
				m_pkNetBridge->set_entity (entity);
			}
		}
	}
}
