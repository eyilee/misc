#include "stdafx.h"
#include "PlayerEntity.h"
#include "protocol/netcommand/NC_ClientJoinGame.h"
#include "protocol/netcommand/NC_ClientGameSnapshot.h"
#include "game/ServerGame.h"

unsigned short CServerGame::ServerTickRate = 20;
unsigned short CServerGame::ClientTickRate = 60;
uint32_t CServerGame::TickStep = 3;
uint64_t CServerGame::TickInterval = 50;

CServerGame::CServerGame ()
	: m_nTick (0)
	, m_nNextTickTime (0)
{
}

CServerGame::~CServerGame ()
{
}

void CServerGame::Init ()
{
	m_kStateMachine.AddState (EGameState::Idle, nullptr, nullptr, nullptr);
	m_kStateMachine.AddState (EGameState::Loading, nullptr, std::bind (&CServerGame::UpdateLoadingState, this), nullptr);
	m_kStateMachine.AddState (EGameState::Active,
		std::bind (&CServerGame::EnterActiveState, this),
		std::bind (&CServerGame::UpdateActiveState, this),
		std::bind (&CServerGame::LeaveActiveState, this));

	m_kStateMachine.SwitchTo (EGameState::Active);
}

void CServerGame::Shutdown ()
{
}

void CServerGame::Update ()
{
	m_kStateMachine.Update ();
}

void CServerGame::Join (std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
{
	uint32_t id = _pkPlayerEntity->GetID ();

	auto it = m_kPlayerEntities.find (id);
	if (it != m_kPlayerEntities.end ()) {
		return;
	}

	m_kPlayerEntities.emplace (id, _pkPlayerEntity);

	OnJoin (_pkPlayerEntity);
}

void CServerGame::Leave (std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
{
	uint32_t id = _pkPlayerEntity->GetID ();

	auto it = m_kPlayerEntities.find (id);
	if (it == m_kPlayerEntities.end ()) {
		return;
	}

	OnLeave (_pkPlayerEntity);

	m_kPlayerEntities.erase (id);
}

void CServerGame::UpdateLoadingState ()
{
}

void CServerGame::EnterActiveState ()
{
	m_nTick = 0;
	m_nNextTickTime = CTime::FrameTime + TickInterval;
}

void CServerGame::UpdateActiveState ()
{
	if (CTime::FrameTime >= m_nNextTickTime)
	{
		for (uint32_t i = 0; i < TickStep; i++) {
			TickUpdate ();
		}

		GenerateSnapshot ();
		BroadcastSnapshot ();

		m_nNextTickTime += TickInterval;
	}
}

void CServerGame::LeaveActiveState ()
{
}

void CServerGame::OnJoin (std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
{
	m_kGameWorld.CreatePlayer (_pkPlayerEntity->GetID ());

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientJoinGame> (m_nID);
	_pkPlayerEntity->GetNetBridge ()->ComposeTcpOutput (protocol);
}

void CServerGame::OnLeave (std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
{
	m_kGameWorld.RemovePlayer (_pkPlayerEntity->GetID ());
}

void CServerGame::TickUpdate ()
{
	m_nTick++;

	// TODO: handle client commands

	// TODO: update systems
}

void CServerGame::GenerateSnapshot ()
{
}

void CServerGame::BroadcastSnapshot ()
{
	for (auto& pair : m_kPlayerEntities)
	{
		uint32_t id = pair.first;
		std::shared_ptr<CPlayerEntity> playerEntity = pair.second;

		//std::shared_ptr<CNetBridge> netBridge = playerEntity->GetNetBridge ();
		//netBridge->ComposeUdpOutput ();
	}
}
