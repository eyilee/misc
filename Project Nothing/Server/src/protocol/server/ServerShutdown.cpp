#include "stdafx.h"

#include "PlayerEntity.h"
#include "ConfigLoader.h"
#include "Server.h"

#include "protocol/server/ServerShutdown.h"

ServerShutdown::ServerShutdown ()
{
}

ServerShutdown::~ServerShutdown ()
{
}

void ServerShutdown::serialize (COutStream& _kOut_Stream)
{
}

void ServerShutdown::deserialize (CInStream& _kIn_Stream)
{
}

void ServerShutdown::excute ()
{
	std::shared_ptr<CPlayerEntity> pPlayer_entity = std::static_pointer_cast<CPlayerEntity> (m_pNet_Bridge->get_entity ());

	if (pPlayer_entity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}

	if (pPlayer_entity->get_id () != 0) {
		LOG_ERROR ("Permission denied.");
	}

	LOG_INFO ("Call shutdown!");

	CServer::Instance->shutdown ();
}
