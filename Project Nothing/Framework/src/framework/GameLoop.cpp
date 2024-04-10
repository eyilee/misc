#include "stdafx.h"
#include "framework/GameLoop.h"

IGameLoop::IGameLoop ()
	: m_nID (0)
	, m_bIsRunning (true)
{
}

IGameLoop::~IGameLoop ()
{
}
