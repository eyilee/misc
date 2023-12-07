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

void ServerEcho::serialize (COutStream& _kOut_stream)
{
}

void ServerEcho::deserialize (CInStream& _kIn_stream)
{
	_kIn_stream >> m_kString;
}

void ServerEcho::excute ()
{
	std::shared_ptr<INetProtocol> pProtocol = std::make_shared<ClientEcho> (m_kString);
	m_pkNetBridge->compose_output (pProtocol);

	std::shared_ptr<CPlayerEntity> pEntity = std::static_pointer_cast<CPlayerEntity> (m_pkNetBridge->get_entity ());

	if (pEntity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}
}
