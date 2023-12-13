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
	if (m_pkNetBridge != nullptr)
	{
		std::shared_ptr<IEntity> entity = CEntityManager::GetOrCreateEntity<CPlayerEntity> (m_nID);
		if (entity != nullptr) {
			entity->SetNetBridge (m_pkNetBridge);
		}

		// tofix: setup udp endpoint by udp session
		short udpport = CConfigLoader::GetConfig<short> ("server.udpport");
		m_pkNetBridge->SetUdpEndPoint (udpport);

		std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientLoginResult> (m_pkNetBridge->GetUdpKey ());
		m_pkNetBridge->ComposeOutput (protocol);
	}
}
