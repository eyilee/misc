#include "stdafx.h"
#include "ServerGame.h"

unsigned short CServerGame::TickRate = 30;

CServerGame::CServerGame ()
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
