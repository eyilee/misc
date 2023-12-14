#include "stdafx.h"
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
	LOG_DEBUG ("Protocol: %hu.", ClientEcho::GetID ());
	m_pkNetBridge->ComposeTcpOutput (protocol);
	m_pkNetBridge->ComposeTcpOutput (protocol);
	m_pkNetBridge->ComposeTcpOutput (protocol);
	m_pkNetBridge->ComposeTcpOutput (protocol);
	m_pkNetBridge->ComposeTcpOutput (protocol);

	std::shared_ptr<IEntity> entity = m_pkNetBridge->GetEntity ();
	if (entity == nullptr) {
		LOG_ERROR ("Entity not found.");
	}
}
