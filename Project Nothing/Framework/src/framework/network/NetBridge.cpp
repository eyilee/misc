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

	CNetworkManager::RemoveNetBridge (GetID ());
}

void CNetBridge::OnDisconnect ()
{
	Shutdown ();
}

void CNetBridge::ComposeTcpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const
{
	if (m_pkTcpConnection == nullptr) {
		return;
	}

	m_pkTcpConnection->ComposeOutput (_pkProtocol);
}

void CNetBridge::ComposeUdpOutput (std::shared_ptr<INetProtocol> _pkProtocol) const
{
	if (m_pkUdpConnection == nullptr) {
		return;
	}

	m_pkUdpConnection->ComposeOutput (_pkProtocol);
}
