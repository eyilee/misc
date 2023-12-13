#include "stdafx.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/NetBridge.h"

CNetBridge::CNetBridge (std::shared_ptr<CTcpSession> _pkSession, const uint32_t& _rnIP)
	: m_pkSession (_pkSession)
	, m_nIP (_rnIP)
	, m_nKey (0)
	, m_pkEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::ResolveInput (CBitInStream& _rkInStream)
{
	unsigned short protocolID;
	_rkInStream.Read (protocolID);

	std::shared_ptr<INetProtocol> protocol = CProtocolManager::GenerateProtocol (protocolID);
	if (protocol != nullptr)
	{
		protocol->SetNetBridge (shared_from_this ());
		protocol->Deserialize (_rkInStream);
		protocol->Excute ();
	}
}

void CNetBridge::ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol)
{
	CBitOutStream outStream;
	_pkProtocol->OnSerialize (outStream);
	m_pkSession->OnWrite (outStream);
}
