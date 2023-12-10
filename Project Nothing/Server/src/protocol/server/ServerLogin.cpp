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

void ServerLogin::serialize (COutStream& _rkOutStream)
{
}

void ServerLogin::deserialize (CInStream& _rkInStream)
{
	_rkInStream >> m_nID;
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
