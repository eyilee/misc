#include "stdafx.h"
#include "PlayerEntity.h"
#include "game/ServerGame.h"
#include "protocol/netcommand/NC_ClientCreateGameResult.h"
#include "protocol/netevent/NE_ServerCreateGame.h"

NE_ServerCreateGame::NE_ServerCreateGame ()
{
}

NE_ServerCreateGame::~NE_ServerCreateGame ()
{
}

void NE_ServerCreateGame::Deserialize (CBitInStream& _rkInStream)
{
}

void NE_ServerCreateGame::Excute ()
{
	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientCreateGameResult> ();
	m_pkNetBridge->ComposeTcpOutput (protocol);

	std::shared_ptr<CServerGame> game = CGameManager::CreateGame<CServerGame> ();
	if (game == nullptr) {
		return;
	}

	std::shared_ptr<CPlayerEntity> playerEntity = std::static_pointer_cast<CPlayerEntity> (m_pkNetBridge->GetNetEntity ());
	if (playerEntity == nullptr) {
		return;
	}

	game->Join (playerEntity);
}
