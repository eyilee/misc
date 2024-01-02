#include "stdafx.h"
#include "ServerGame.h"
#include "protocol/command/NC_ClientCreateGameResult.h"
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
	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientCreateGameResult> ();
	m_pkNetBridge->ComposeTcpOutput (protocol);

	/*std::shared_ptr<CServerGame> game = std::make_shared<CServerGame> ();
	game->Init ();
	CGameManager::AddGame (game);*/
}
