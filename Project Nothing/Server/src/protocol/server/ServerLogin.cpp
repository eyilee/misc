#include "stdafx.h"
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
	if (m_pkNetBridge != nullptr)
	{
		std::shared_ptr<IEntity> entity = CEntityManager::GetOrCreateEntity<CPlayerEntity> (m_nID);
		if (entity != nullptr) {
			entity->SetNetBridge (m_pkNetBridge);
		}

		m_pkNetBridge->SetKey (0);
		m_pkNetBridge->SetEntity (entity);

		std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientLoginResult> ();
		m_pkNetBridge->ComposeOutput (protocol);
	}
}
