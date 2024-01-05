#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/TcpConnection.h"

CTcpConnection::CTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CTcpSession> _pkTcpSession)
	: m_pkNetBridge (_pkNetBridge)
	, m_pkTcpSession (_pkTcpSession)
	, m_kEndPoint (_pkTcpSession->GetEndpoint ())
{
}

CTcpConnection::~CTcpConnection ()
{
}

void CTcpConnection::Init ()
{
	m_pkTcpSession->Init (shared_from_this ());
}

void CTcpConnection::Shutdown ()
{
	m_pkNetBridge = nullptr;

	m_pkTcpSession->Shutdown ();
	m_pkTcpSession = nullptr;
}

void CTcpConnection::ResolveInput (CBitInStream& _rkInStream)
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

void CTcpConnection::ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol)
{
	CBitOutStream outStream;
	_pkProtocol->OnSerialize (outStream);
	m_pkTcpSession->Write (outStream);
}
