#include "stdafx.h"
#include "PlayerEntity.h"
#include "Server.h"
#include "protocol/server/ServerShutdown.h"

ServerShutdown::ServerShutdown ()
{
}

ServerShutdown::~ServerShutdown ()
{
}

void ServerShutdown::serialize (COutStream& _kOut_stream)
{
}

void ServerShutdown::deserialize (CInStream& _kIn_stream)
{
}

void ServerShutdown::excute ()
{
	std::shared_ptr<CPlayerEntity> entity = std::static_pointer_cast<CPlayerEntity> (m_pkNetBridge->get_entity ());

	if (entity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}

	if (entity->get_id () != 0) {
		LOG_ERROR ("Permission denied.");
	}

	LOG_INFO ("Call shutdown!");

	if (CServer::Instance != nullptr) {
		CServer::Instance->shutdown ();
	}
}
