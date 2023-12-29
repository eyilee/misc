#include "stdafx.h"
#include "ServerGame.h"
#include "protocol/event/NE_ServerCreateGame.h"

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
	std::shared_ptr<CServerGame> game = std::make_shared<CServerGame> ();
	game->Init ();
	CGameManager::AddGame (game);
}
