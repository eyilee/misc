#include "stdafx.h"
#include "ConfigLoader.h"
#include "PlayerEntity.h"
#include "protocol/netcommand/NC_ClientLoginResult.h"
#include "protocol/netevent/NE_ServerLogin.h"

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
	std::shared_ptr<INetEntity> netEntity = CNetEntityManager::GetOrCreateNetEntity<CPlayerEntity> (m_nID);
	if (netEntity != nullptr)
	{
		netEntity->SetNetBridge (m_pkNetBridge);
		m_pkNetBridge->SetNetEntity (netEntity);
	}

	uint32_t id = m_pkNetBridge->GetID ();
	uint32_t key = CRandom::GetValue<uint32_t> ();
	m_pkNetBridge->SetKey (key);

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientLoginResult> (id, key);
	m_pkNetBridge->ComposeTcpOutput (protocol);
}
