#include "stdafx.h"
#include "game/GameObject.h"
#include "game/GameWorld.h"

CGameWorld::CGameWorld ()
{
}

CGameWorld::~CGameWorld ()
{
}

void CGameWorld::CreatePlayer (uint32_t _nID)
{
	auto it = m_kGameObjects.find (_nID);
	if (it != m_kGameObjects.end ()) {
		return;
	}

	std::shared_ptr<IGameObject> player = std::make_shared<IGameObject> ();
	player->SetID (_nID);
	player->SetPosition (0, 0);
	m_kGameObjects.emplace (_nID, player);
}

void CGameWorld::RemovePlayer (uint32_t _nID)
{
	auto it = m_kGameObjects.find (_nID);
	if (it == m_kGameObjects.end ()) {
		return;
	}

	m_kGameObjects.erase (_nID);
}
