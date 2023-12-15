#include "stdafx.h"
#include "ConfigLoader.h"
#include "PlayerEntity.h"
#include "protocol/client/ClientLoginResult.h"
#include "protocol/server/ServerLogin.h"

ServerLogin::ServerLogin ()
	: m_nID (0)
{
}

ServerLogin::~ServerLogin ()
{
}

void ServerLogin::Serialize (CBitOutStream& _rkOutStream)
{
}

void ServerLogin::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nID);
}

void ServerLogin::Excute ()
{
	std::shared_ptr<IEntity> entity = CEntityManager::GetOrCreateEntity<CPlayerEntity> (m_nID);
	if (entity != nullptr)
	{
		entity->SetNetBridge (m_pkNetBridge);
		m_pkNetBridge->SetEntity (entity);
	}

	uint32_t key = CRandom::GetValue<uint32_t> ();
	m_pkNetBridge->SetUdpKey (key);

	std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientLoginResult> (m_nID, key);
	m_pkNetBridge->ComposeTcpOutput (protocol);
}
