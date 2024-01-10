#include "stdafx.h"
#include "PlayerEntity.h"
#include "game/ServerConnection.h"
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

bool CServerGame::Join (std::shared_ptr<CPlayerEntity> _pkPlayerEntity, std::shared_ptr<CUdpSession> _pkUdpSession)
{
	uint32_t playerID = _pkPlayerEntity->GetID ();

	auto it = m_kServerConnections.find (playerID);
	if (it != m_kServerConnections.end ()) {
		return false;
	}

	std::shared_ptr<CServerConnection> serverConnection = std::make_shared<CServerConnection> (_pkUdpSession, shared_from_this (), _pkPlayerEntity);

	m_kServerConnections.emplace (playerID, serverConnection);

	OnJoin (serverConnection);

	return true;
}

void CServerGame::Leave (std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
{
	uint32_t playerID = _pkPlayerEntity->GetID ();

	auto it = m_kServerConnections.find (playerID);
	if (it == m_kServerConnections.end ()) {
		return;
	}

	OnLeave (it->second);

	m_kServerConnections.erase (it);
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

void CServerGame::OnJoin (std::shared_ptr<CServerConnection> _pkConnection)
{
	_pkConnection->Init ();

	uint32_t playerID = _pkConnection->GetPlayerID ();
	if (playerID != 0) {
		m_kGameWorld.CreatePlayer (_pkConnection->GetPlayerID ());
	}
}

void CServerGame::OnLeave (std::shared_ptr<CServerConnection> _pkConnection)
{
	uint32_t playerID = _pkConnection->GetPlayerID ();
	if (playerID != 0) {
		m_kGameWorld.RemovePlayer (_pkConnection->GetPlayerID ());
	}

	_pkConnection->Shutdown ();
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
	for (auto& pair : m_kServerConnections)
	{
		//std::shared_ptr<CNetBridge> netBridge = playerEntity->GetNetBridge ();
		//netBridge->ComposeUdpOutput ();
	}
}
