#include "stdafx.h"
#include "PlayerEntity.h"
#include "game/ClientInfo.h"
#include "game/ServerConnection.h"
#include "game/ServerGame.h"

unsigned short CServerGame::ServerTickRate = 20;
unsigned short CServerGame::ClientTickRate = 60;
uint32_t CServerGame::TickStep = 3;
uint64_t CServerGame::TickInterval = 50;

CServerGame::CServerGame ()
	: m_nTick (0)
	, m_nNextTickTime (0)
	, m_nServerSequence ()
	, m_kSnapshots {}
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
	SetRunning (false);
}

void CServerGame::Update ()
{
	m_kStateMachine.Update ();
}

bool CServerGame::Join (std::shared_ptr<CPlayerEntity> _pkPlayerEntity, std::shared_ptr<CUdpSession> _pkUdpSession)
{
	if (_pkPlayerEntity->GetGameID () != 0) {
		return false;
	}

	uint32_t playerID = _pkPlayerEntity->GetID ();

	auto it = m_kServerConnections.find (playerID);
	if (it != m_kServerConnections.end ()) {
		return false;
	}

	std::shared_ptr<CServerConnection> serverConnection = std::make_shared<CServerConnection> (_pkUdpSession, shared_from_this (), _pkPlayerEntity);

	m_kServerConnections.emplace (playerID, serverConnection);

	OnConnected (serverConnection);

	_pkPlayerEntity->SetGameID (m_nID);

	return true;
}

void CServerGame::Leave (std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
{
	uint32_t playerID = _pkPlayerEntity->GetID ();

	auto it = m_kServerConnections.find (playerID);
	if (it == m_kServerConnections.end ()) {
		return;
	}

	std::shared_ptr<CServerConnection> serverConnection = it->second;

	m_kServerConnections.erase (it);

	OnDisconnected (it->second);

	_pkPlayerEntity->SetGameID (0);

	if (m_kServerConnections.empty ()) {
		Shutdown ();
	}
}

void CServerGame::ProcessCommand (uint32_t _nPlayerID, const SUserCommand& _rkCommand, uint32_t _nTick)
{
	auto it = m_kClientInfos.find (_nPlayerID);
	if (it == m_kClientInfos.end ()) {
		return;
	}

	std::shared_ptr<CClientInfo> clientInfo = it->second;

	if (_nTick == m_nTick) {
		clientInfo->m_kLatestCommand = _rkCommand;
	}

	// TODO: store commands to systems
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
	while (CTime::FrameTime >= m_nNextTickTime)
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

void CServerGame::OnConnected (std::shared_ptr<CServerConnection> _pkConnection)
{
	_pkConnection->Init ();

	uint32_t playerID = _pkConnection->GetPlayerID ();
	if (playerID != 0) {
		OnJoin (playerID);
	}
}

void CServerGame::OnDisconnected (std::shared_ptr<CServerConnection> _pkConnection)
{
	uint32_t playerID = _pkConnection->GetPlayerID ();
	if (playerID != 0) {
		OnLeave (playerID);
	}

	_pkConnection->Shutdown ();
}

void CServerGame::OnJoin (uint32_t _nPlayerID)
{
	auto it = m_kClientInfos.find (_nPlayerID);
	if (it != m_kClientInfos.end ()) {
		return;
	}

	m_kClientInfos.emplace (_nPlayerID, std::make_shared<CClientInfo> ());

	m_kGameWorld.CreatePlayer (_nPlayerID);
}

void CServerGame::OnLeave (uint32_t _nPlayerID)
{
	auto it = m_kClientInfos.find (_nPlayerID);
	if (it == m_kClientInfos.end ()) {
		return;
	}

	m_kClientInfos.erase (it);

	m_kGameWorld.RemovePlayer (_nPlayerID);
}

void CServerGame::TickUpdate ()
{
	m_nTick++;

	// TODO: handle client commands
	for (auto& [id, connection] : m_kServerConnections) {
		connection->ProcessCommands (m_nTick);
	}

	// TODO: update systems
}

void CServerGame::GenerateSnapshot ()
{
}

void CServerGame::BroadcastSnapshot ()
{
	for (auto& [id, connection] : m_kServerConnections) {
		connection->ComposePackage ();
	}
}
