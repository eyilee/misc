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
	uint32_t gameID = 0;

	std::shared_ptr<CServerGame> game = CGameManager::CreateGame<CServerGame> ();
	if (game != nullptr) {
		gameID = game->GetID ();
		game->Init (); // TODO: for test, remove
	}

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientCreateGameResult> (gameID);
	m_pkNetBridge->ComposeOutput (protocol);
}
