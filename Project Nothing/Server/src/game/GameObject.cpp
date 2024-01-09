#include "stdafx.h"
#include "game/GameObject.h"

IGameObject::IGameObject ()
	: m_nID (0)
	, m_nX (0)
	, m_nY (0)
{
}

IGameObject::~IGameObject ()
{
}

void IGameObject::SetPosition (float _nX, float _nY)
{
	m_nX = _nX;
	m_nY = _nY;
}
