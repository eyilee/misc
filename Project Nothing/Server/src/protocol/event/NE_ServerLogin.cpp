#include "stdafx.h"
#include "ConfigLoader.h"
#include "PlayerEntity.h"
#include "protocol/command/NC_ClientLoginResult.h"
#include "protocol/event/NE_ServerLogin.h"

NE_ServerLogin::NE_ServerLogin ()
	: m_nID (0)
{
}

NE_ServerLogin::~NE_ServerLogin ()
{
}

void NE_ServerLogin::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nID);
}

void NE_ServerLogin::Excute ()
{
	std::shared_ptr<IEntity> entity = CEntityManager::GetOrCreateEntity<CPlayerEntity> (m_nID);
	if (entity != nullptr)
	{
		entity->SetNetBridge (m_pkNetBridge);
		m_pkNetBridge->SetEntity (entity);
	}

	uint32_t key = CRandom::GetValue<uint32_t> ();
	m_pkNetBridge->SetKey (key);

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientLoginResult> (m_nID, key);
	m_pkNetBridge->ComposeTcpOutput (protocol);
}
