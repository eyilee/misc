#include "stdafx.h"
#include "framework/network/Entity.h"
#include "framework/network/InStream.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/OutStream.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/BaseManager.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/NetBridge.h"

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
	COutStream kOut_stream;
	_pNet_protocol->on_serialize (kOut_stream);
	m_pSession->on_write (kOut_stream);
}
