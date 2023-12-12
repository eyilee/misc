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

void ServerEcho::Serialize (CBitOutStream& _rkOutStream)
{
}

void ServerEcho::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_kString);
}

void ServerEcho::Excute ()
{
	std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientEcho> (m_kString);
	m_pkNetBridge->ComposeOutput (protocol);

	std::shared_ptr<CPlayerEntity> entity = std::static_pointer_cast<CPlayerEntity> (m_pkNetBridge->GetEntity ());

	if (entity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}
}
