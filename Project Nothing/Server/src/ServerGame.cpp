#include "stdafx.h"
#include "ServerGame.h"

unsigned short CServerGame::ServerTickRate = 20;
unsigned short CServerGame::ClientTickRate = 60;
uint32_t CServerGame::TickStep = 3;
uint64_t CServerGame::TickInterval = 50;

CServerGame::CServerGame ()
	: Tick (0)
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

	m_kStateMachine.SwitchTo (EGameState::Idle);
}

void CServerGame::Shutdown ()
{
}

void CServerGame::Update ()
{
}

void CServerGame::LateUpdate ()
{
}

void CServerGame::UpdateLoadingState ()
{
}

void CServerGame::EnterActiveState ()
{
}

void CServerGame::UpdateActiveState ()
{
}

void CServerGame::LeaveActiveState ()
{
}
