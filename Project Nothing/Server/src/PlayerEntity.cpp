#include "stdafx.h"
#include "game/ServerGame.h"
#include "PlayerEntity.h"

CPlayerEntity::CPlayerEntity ()
	: m_nGameID (0)
{
}

CPlayerEntity::~CPlayerEntity ()
{
}

void CPlayerEntity::OnConnected ()
{
}

void CPlayerEntity::OnDisconnected ()
{
	std::shared_ptr<CServerGame> game = CGameManager::GetGame<CServerGame> (m_nGameID);
	if (game != nullptr) {
		game->Leave (shared_from_this ());
	}
}
