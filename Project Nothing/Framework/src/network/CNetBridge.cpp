#include "stdafx.h"

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

void CNetBridge::resolve_input (CInStream& _kIn_Stream)
{
	unsigned short nProtocol_id;
	_kIn_Stream >> nProtocol_id;

	auto self (shared_from_this ());
	std::shared_ptr<INetProtocol> pNet_protocol = generate_protocol (nProtocol_id, self);
	pNet_protocol->deserialize (_kIn_Stream);
	pNet_protocol->excute ();
}

void CNetBridge::compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol)
{
	COutStream kOut_Stream;
	_pNet_protocol->on_serialize (kOut_Stream);
	m_pSession->on_write (kOut_Stream);
}

std::shared_ptr<INetProtocol> CNetBridge::generate_protocol (unsigned short _nProtocol_id, std::shared_ptr<CNetBridge>& _pNetBridge)
{
	std::shared_ptr<CProtocolManager> pProtocol_manager = CProtocolManager::Instance;

	return pProtocol_manager->generate_protocol (_nProtocol_id, _pNetBridge);
}
