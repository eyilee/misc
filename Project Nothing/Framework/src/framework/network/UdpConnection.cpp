#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/UdpConnection.h"

CUdpConnection::CUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CUdpSession> _pkUdpSession)
	: m_pkNetBridge (_pkNetBridge)
	, m_pkUdpSession (_pkUdpSession)
	, m_kUdpEndPoint (_pkUdpSession->m_kEndpoint)
{
}

CUdpConnection::~CUdpConnection ()
{
}

void CUdpConnection::Init ()
{
}

void CUdpConnection::Shutdown ()
{
	m_pkNetBridge = nullptr;
	m_pkUdpSession = nullptr;
}

void CUdpConnection::ResolveInput (CBitInStream& _rkInStream)
{
	unsigned short protocolID;
	_rkInStream.Read (protocolID);

	std::shared_ptr<INetProtocol> protocol = CProtocolManager::GenerateProtocol (protocolID);
	if (protocol != nullptr)
	{
		protocol->SetNetBridge (m_pkNetBridge);
		protocol->Deserialize (_rkInStream);
		protocol->Excute ();
	}
}

void CUdpConnection::ComposeOutput (CBitOutStream& _rkOutStream)
{
	m_pkUdpSession->Send (_rkOutStream, m_kUdpEndPoint);
}
