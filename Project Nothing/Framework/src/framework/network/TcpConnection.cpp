#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/TcpSession.h"
#include "framework/network/TcpConnection.h"

CTcpConnection::CTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket)
	: m_pkNetBridge (_pkNetBridge)
{
	m_pkTcpSession = std::make_shared<CTcpSession> (_rkSocket);
	m_nIP = m_pkTcpSession->GetIP ();
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
	m_pkNetBridge->ResolveInput (_rkInStream);
}

void CTcpConnection::ComposeOutput (CBitOutStream& _rkOutStream)
{
	m_pkTcpSession->OnWrite (_rkOutStream);
}
