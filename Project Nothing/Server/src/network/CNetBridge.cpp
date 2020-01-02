#include "stdafx.h"

#include "CInStream.h"
#include "COutStream.h"
#include "CEntity.h"
#include "CTcpSession.h"
#include "protocol/INetProtocol.h"
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

	std::shared_ptr<INetProtocol> pNet_protocol = generate_protocol (nProtocol_id);

	pNet_protocol->deserialize (_kIn_Stream);
	pNet_protocol->excute ();
}

void CNetBridge::compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol)
{
	COutStream kOut_Stream;
	kOut_Stream << _pNet_protocol->get_protocol_id ();

	_pNet_protocol->serialize (kOut_Stream);
	m_pSession->on_write (kOut_Stream);
}

std::shared_ptr<INetProtocol> CNetBridge::generate_protocol (unsigned short _nProtocol_id)
{
	return std::make_shared<INetProtocol> ();
}
