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

void ServerShutdown::Serialize (CBitOutStream& _rkOutStream)
{
}

void ServerShutdown::Deserialize (CBitInStream& _rkInStream)
{
}

void ServerShutdown::Excute ()
{
	std::shared_ptr<CPlayerEntity> entity = std::static_pointer_cast<CPlayerEntity> (m_pkNetBridge->GetEntity ());

	if (entity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}

	if (entity->GetID () != 0) {
		LOG_ERROR ("Permission denied.");
	}

	LOG_INFO ("Call shutdown!");

	if (CServer::Instance != nullptr) {
		CServer::Instance->Shutdown ();
	}
}
