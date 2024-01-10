#include "stdafx.h"
#include "framework/Random.h"
#include "framework/network/NetEntity.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpConnection.h"
#include "framework/manager/NetworkManager.h"
#include "framework/network/NetBridge.h"

CNetBridge::CNetBridge (uint32_t _nID)
	: m_nID (_nID)
	, m_pkConnection (nullptr)
	, m_pkNetEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::Init (std::shared_ptr<CTcpConnection> _pkConnection)
{
	m_pkConnection = _pkConnection;
}

void CNetBridge::Shutdown ()
{
	if (m_pkConnection != nullptr)
	{
		m_pkConnection->Shutdown ();
		m_pkConnection = nullptr;
	}

	if (m_pkNetEntity != nullptr)
	{
		m_pkNetEntity->SetNetBridge (nullptr);
		m_pkNetEntity = nullptr;
	}

	CNetworkManager::RemoveNetBridge (m_nID);
}

void CNetBridge::OnDisconnect ()
{
	Shutdown ();
}

void CNetBridge::ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol) const
{
	if (m_pkConnection == nullptr) {
		return;
	}

	m_pkConnection->ComposeOutput (_pkProtocol);
}
