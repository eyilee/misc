#include "stdafx.h"
#include "PlayerEntity.h"
#include "Server.h"
#include "protocol/event/NE_ServerShutdown.h"

NE_ServerShutdown::NE_ServerShutdown ()
{
}

NE_ServerShutdown::~NE_ServerShutdown ()
{
}

void NE_ServerShutdown::Serialize (CBitOutStream& _rkOutStream)
{
}

void NE_ServerShutdown::Deserialize (CBitInStream& _rkInStream)
{
}

void NE_ServerShutdown::Excute ()
{
	std::shared_ptr<IEntity> entity = m_pkNetBridge->GetEntity ();
	if (entity == nullptr) {
		LOG_ERROR ("Entity not found.");
	}

	CServer::Shutdown ();
}
