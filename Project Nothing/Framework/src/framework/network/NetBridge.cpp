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
	, m_nIP (0)
	, m_nKey (0)
	, m_pkEntity (nullptr)
{
}

CNetBridge::~CNetBridge ()
{
}

void CNetBridge::SetTcpConnection (std::shared_ptr<CTcpConnection> _pkTcpConnection)
{
	m_pkTcpConnection = _pkTcpConnection;
	m_nIP = m_pkTcpConnection->GetIP ();
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

uint32_t CNetBridge::GetIP ()
{
	if (m_pkTcpConnection != nullptr) {
		return m_pkTcpConnection->GetIP ();
	}

	return 0;
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
