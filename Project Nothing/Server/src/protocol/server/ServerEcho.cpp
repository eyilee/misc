#include "stdafx.h"

#include "CPlayerEntity.h"

#include "protocol/client/ClientEcho.h"
#include "protocol/server/ServerEcho.h"

ServerEcho::ServerEcho ()
{
}

ServerEcho::~ServerEcho ()
{
}

void ServerEcho::serialize (COutStream& _kOut_Stream)
{
}

void ServerEcho::deserialize (CInStream& _kIn_Stream)
{
	_kIn_Stream >> m_kString;
}

void ServerEcho::excute ()
{
	std::shared_ptr<INetProtocol> pProtocol = std::make_shared<ClientEcho> (m_kString);
	m_pNet_Bridge->compose_output (pProtocol);

	std::shared_ptr<CPlayerEntity> pEntity = std::static_pointer_cast<CPlayerEntity> (m_pNet_Bridge->get_entity ());

	if (pEntity == nullptr) {
		LOG_ERROR ("Player entity not found.");
	}
}
