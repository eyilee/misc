#include "stdafx.h"
#include "PlayerEntity.h"
#include "Server.h"
#include "protocol/netevent/NE_ServerShutdown.h"

NE_ServerShutdown::NE_ServerShutdown ()
{
}

NE_ServerShutdown::~NE_ServerShutdown ()
{
}

void NE_ServerShutdown::Deserialize (CBitInStream& _rkInStream)
{
}

void NE_ServerShutdown::Excute ()
{
	std::shared_ptr<INetEntity> netEntity = m_pkNetBridge->GetNetEntity ();
	if (netEntity == nullptr) {
		LOG_ERROR ("NetEntity not found.");
	}

	CServer::Shutdown ();
}
