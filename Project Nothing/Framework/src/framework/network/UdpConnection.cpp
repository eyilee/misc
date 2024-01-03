#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/UdpSession.h"
#include "framework/network/UdpConnection.h"

CUdpConnection::CUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CUdpSession> _pkUdpSession)
	: m_pkNetBridge (_pkNetBridge)
	, m_pkUdpSession (_pkUdpSession)
	, m_kUdpEndPoint (_pkUdpSession->GetEndPoint ())
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

void CUdpConnection::ComposeOutput (CBitOutStream& _rkOutStream)
{
	m_pkUdpSession->OnSend (_rkOutStream, m_kUdpEndPoint);
}
