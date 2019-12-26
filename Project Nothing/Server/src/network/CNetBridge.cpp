#include "stdafx.h"

#include "CEntity.h"
#include "CTcpSession.h"
#include "CNetBridge.h"

CNetBridge::CNetBridge (std::shared_ptr<CTcpSession>& _pSession)
	: m_pSession (_pSession)
	, m_pEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::set_entity (std::shared_ptr<CEntity>& _pEntity)
{
	m_pEntity = _pEntity;
}

std::shared_ptr<CEntity>& CNetBridge::get_entity ()
{
	return m_pEntity;
}
