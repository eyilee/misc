#include "stdafx.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/NetBridge.h"

CNetBridge::CNetBridge (std::shared_ptr<CTcpSession> _pkSession)
	: m_pkSession (_pkSession)
	, m_pkEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::set_entity (std::shared_ptr<IEntity> _pkEntity)
{
	m_pkEntity = _pkEntity;
}

std::shared_ptr<IEntity> CNetBridge::get_entity ()
{
	return m_pkEntity;
}

void CNetBridge::resolve_input (CBitInStream& _rkInStream)
{
	unsigned short protocolID;
	_rkInStream.Read (protocolID);

	if (CProtocolManager::Instance != nullptr)
	{
		std::shared_ptr<INetProtocol> protocol = CProtocolManager::Instance->generate_protocol (protocolID);
		protocol->set_net_bridge (shared_from_this ());
		protocol->deserialize (_rkInStream);
		protocol->excute ();
	}
}

void CNetBridge::compose_output (std::shared_ptr<INetProtocol> _pkProtocol)
{
	CBitOutStream outStream;
	_pkProtocol->on_serialize (outStream);
	m_pkSession->on_write (outStream);
}
