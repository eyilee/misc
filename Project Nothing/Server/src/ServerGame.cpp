#include "stdafx.h"
#include "ServerGame.h"

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
}
