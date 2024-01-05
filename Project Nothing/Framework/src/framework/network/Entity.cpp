#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/Entity.h"

IEntity::IEntity ()
	: m_nID (0)
	, m_pkNetBridge (nullptr)
{
}

IEntity::~IEntity ()
{
}
