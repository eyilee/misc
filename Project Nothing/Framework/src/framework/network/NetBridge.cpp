#include "stdafx.h"
#include "framework/Random.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpConnection.h"
#include "framework/network/UdpConnection.h"
#include "framework/manager/NetworkManager.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/network/NetBridge.h"

CNetBridge::CNetBridge ()
	: m_pkTcpConnection (nullptr)
	, m_pkUdpConnection (nullptr)
	, m_nID (0)
	, m_nIP (0)
	, m_nKey (0)
	, m_pkEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::Shutdown ()
{
	if (m_pkTcpConnection != nullptr)
	{
		m_pkTcpConnection->Shutdown ();
		m_pkTcpConnection = nullptr;
	}

	if (m_pkUdpConnection != nullptr)
	{
		m_pkUdpConnection->Shutdown ();
		m_pkUdpConnection = nullptr;
	}

	if (m_pkEntity != nullptr)
	{
		m_pkEntity->SetNetBridge (nullptr);
		m_pkEntity = nullptr;
	}
}

void CNetBridge::SetTcpConnection (std::shared_ptr<CTcpConnection> _pkTcpConnection)
{
	m_pkTcpConnection = _pkTcpConnection;
	m_nIP = m_pkTcpConnection->GetIP ();
}

void CNetBridge::SetUdpConnection (std::shared_ptr<CUdpConnection> _pkUdpConnection)
{
	m_pkUdpConnection = _pkUdpConnection;
}

void CNetBridge::ResolveUdpInput (uint32_t _nIP, uint32_t _nKey, CBitInStream& _rkInStream)
{
	if (m_nIP != _nIP || m_nKey != _nKey) {
		return;
	}

	if (m_pkUdpConnection == nullptr) {
		return;
	}

	m_pkUdpConnection->ResolveInput (_rkInStream);
}

void CNetBridge::ComposeTcpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const
{
	if (m_pkTcpConnection == nullptr) {
		return;
	}

	CBitOutStream outStream;
	_pkProtocol->OnSerialize (outStream);
	m_pkTcpConnection->ComposeOutput (outStream);
}

void CNetBridge::ComposeUdpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const
{
	if (m_pkUdpConnection != nullptr) {
		return;
	}

	CBitOutStream outStream;
	outStream.Write (m_nKey);
	_pkProtocol->OnSerialize (outStream);
	m_pkUdpConnection->ComposeOutput (outStream);
}
