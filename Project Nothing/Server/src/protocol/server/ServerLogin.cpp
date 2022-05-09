#include "stdafx.h"

#include "PlayerEntity.h"

#include "protocol/server/ServerLogin.h"

ServerLogin::ServerLogin ()
	: m_nId (0)
{
}

ServerLogin::~ServerLogin ()
{
}

void ServerLogin::serialize (COutStream& _kOut_stream)
{
}

void ServerLogin::deserialize (CInStream& _kIn_stream)
{
	_kIn_stream >> m_nId;
}

void ServerLogin::excute ()
{
	std::shared_ptr<CPlayerEntity> pPlayer_entity = CEntityManager::Instance->create_entity<CPlayerEntity> (m_nId);

	pPlayer_entity->set_id (m_nId);

	if (m_pNet_bridge->get_entity () == nullptr) {
		m_pNet_bridge->set_entity (pPlayer_entity);
	}
}
