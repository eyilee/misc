#include "stdafx.h"

#include "framework/network/CEntity.h"
#include "framework/network/CInStream.h"
#include "framework/network/CNetProtocol.h"
#include "framework/network/COutStream.h"
#include "framework/network/CTcpSession.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CProtocolManager.h"

#include "framework/network/CNetBridge.h"

CNetBridge::CNetBridge (std::shared_ptr<CTcpSession>&& _pSession)
	: m_pSession (_pSession)
	, m_pEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::set_entity (std::shared_ptr<IEntity>&& _pEntity)
{
	m_pEntity = _pEntity;
}

std::shared_ptr<IEntity> CNetBridge::get_entity ()
{
	return m_pEntity;
}

void CNetBridge::resolve_input (CInStream& _kIn_Stream)
{
	unsigned short nProtocol_id;
	_kIn_Stream >> nProtocol_id;

	std::shared_ptr<INetProtocol> pNet_protocol = CProtocolManager::Instance->generate_protocol (nProtocol_id);
	pNet_protocol->set_net_bridge (shared_from_this ());
	pNet_protocol->deserialize (_kIn_Stream);
	pNet_protocol->excute ();
}

void CNetBridge::compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol)
{
	COutStream kOut_Stream;
	_pNet_protocol->on_serialize (kOut_Stream);
	m_pSession->on_write (kOut_Stream);
}
