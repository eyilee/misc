#include "stdafx.h"
#include "PlayerEntity.h"
#include "protocol/client/ClientLoginResult.h"
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
		std::shared_ptr<CPlayerEntity> entity = std::static_pointer_cast<CPlayerEntity> (CEntityManager::Instance->get_entity (m_nID));
		if (entity == nullptr)
		{
			entity = std::make_shared<CPlayerEntity> ();
			entity->set_id (m_nID);
			CEntityManager::Instance->set_entity (m_nID, entity);
		}

		if (m_pkNetBridge != nullptr)
		{
			m_pkNetBridge->set_entity (entity);
			entity->set_net_bridge (m_pkNetBridge);

			std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientLoginResult> ();
			m_pkNetBridge->compose_output (protocol);
		}
	}
}
