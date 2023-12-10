#include "stdafx.h"
#include "PlayerEntity.h"
#include "protocol/client/ClientEcho.h"
#include "protocol/server/ServerEcho.h"

ServerEcho::ServerEcho ()
{
}

ServerEcho::~ServerEcho ()
{
}

void ServerEcho::serialize (COutStream& _rkOutStream)
{
}

void ServerEcho::deserialize (CInStream& _rkInStream)
{
	_rkInStream >> m_kString;
}

void ServerEcho::excute ()
{
	std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientEcho> (m_kString);
	m_pkNetBridge->compose_output (protocol);

	std::shared_ptr<CPlayerEntity> entity = std::static_pointer_cast<CPlayerEntity> (m_pkNetBridge->get_entity ());

	if (entity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}
}
