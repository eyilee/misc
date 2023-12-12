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
	if (m_pkNetBridge != nullptr)
	{
		if (CEntityManager::Instance != nullptr)
		{
			std::shared_ptr<IEntity> entity = CEntityManager::Instance->get_or_create_entity<CPlayerEntity> (m_nID);
			if (entity != nullptr) {
				entity->set_net_bridge (m_pkNetBridge);
			}

			m_pkNetBridge->set_key (0);
			m_pkNetBridge->set_entity (entity);

			std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientLoginResult> ();
			m_pkNetBridge->compose_output (protocol);
		}
	}
}
