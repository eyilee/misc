#include "stdafx.h"
#include "PlayerEntity.h"
#include "game/ServerConnection.h"
#include "game/ServerGame.h"
#include "protocol/netcommand/NC_ClientJoinGameResult.h"
#include "protocol/netevent/NE_ServerJoinGame.h"

NE_ServerJoinGame::NE_ServerJoinGame ()
	: m_nGameID (0)
	, m_nPort (0)
{
}

NE_ServerJoinGame::~NE_ServerJoinGame ()
{
}

void NE_ServerJoinGame::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nGameID);
	_rkInStream.Read (m_nPort);
}

void NE_ServerJoinGame::Excute ()
{
	uint32_t gameID = 0;
	unsigned short port = 0;

	std::shared_ptr<CServerGame> game = CGameManager::GetGame<CServerGame> (m_nGameID);
	if (game != nullptr)
	{
		std::shared_ptr<CPlayerEntity> playerEntity = m_pkNetBridge->GetNetEntity<CPlayerEntity> ();
		if (playerEntity == nullptr) {
			return;
		}

		std::shared_ptr<CUdpSession> udpSession = CNetworkManager::UdpConnect (m_pkNetBridge, m_nPort);
		if (udpSession == nullptr) {
			return;
		}

		//if (game->Join (playerEntity, udpSession)) {
		//
		//}

		gameID = game->GetID ();
		port = udpSession->GetLocalEndpoint ().port ();
	}

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientJoinGameResult> (gameID, port);
	m_pkNetBridge->ComposeOutput (protocol);
}
