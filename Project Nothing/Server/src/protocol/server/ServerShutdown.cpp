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
	std::shared_ptr<IEntity> entity = m_pkNetBridge->GetEntity ();
	if (entity == nullptr) {
		LOG_ERROR ("Entity not found.");
	}

	CServer::Shutdown ();
}
