#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetEntity.h"

INetEntity::INetEntity ()
	: m_nID (0)
	, m_pkNetBridge (nullptr)
{
}

INetEntity::~INetEntity ()
{
}
