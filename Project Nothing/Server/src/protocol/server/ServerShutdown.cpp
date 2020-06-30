#include "stdafx.h"

#include "CPlayerEntity.h"

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
		std::cout << "Player entity not found." << std::endl;
	}

	if (pPlayer_entity->get_id () != 0) {
		std::cout << "Permission denied." << std::endl;
	}

	std::cout << "Call shutdown!" << std::endl;
}
