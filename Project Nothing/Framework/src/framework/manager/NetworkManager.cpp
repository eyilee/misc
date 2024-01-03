#include "stdafx.h"
#include "framework/network/NetBridge.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpConnection.h"
#include "framework/network/UdpConnection.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/NetworkManager.h"

CNetworkManager::CNetworkManager ()
	: m_pkListener (nullptr)
	, m_pkUdpSession (nullptr)
	, m_nConnectionID (0)
{
}

CNetworkManager::~CNetworkManager ()
{
}

void CNetworkManager::Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CNetworkManager> ();
	Instance->Run (_rkContext, _rkHostAddr, _nTcpPort, _nUdpPort);
}

void CNetworkManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Stop ();
	Instance = nullptr;
}

void CNetworkManager::TcpAccept (tcp::socket& _rkSocket)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->SetupTcpConnection (_rkSocket);
}

void CNetworkManager::UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->SetupUdpConnection (_pkNetBridge);
}

void CNetworkManager::Run (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort)
{
	m_pkListener = std::make_shared<CTcpListener> (_rkContext, _rkHostAddr, _nTcpPort);
	m_pkListener->Init ();

	m_pkUdpSession = std::make_shared<CUdpSession> (_rkContext, _rkHostAddr, _nUdpPort);
	m_pkUdpSession->Init ();
}

void CNetworkManager::Stop ()
{
	m_pkListener->Shutdown ();

	for (auto& pair : m_pkNetBridges) {
		pair.second->Shutdown ();
	}

	m_pkUdpSession->Shutdown ();
}

void CNetworkManager::SetupTcpConnection (tcp::socket& _rkSocket)
{
	std::shared_ptr<CNetBridge> netBridge = std::make_shared<CNetBridge> ();

	std::shared_ptr<CTcpConnection> connection = std::make_shared<CTcpConnection> (netBridge, _rkSocket);
	connection->Init ();
	netBridge->SetTcpConnection (connection);

	m_nConnectionID++;
	m_pkNetBridges.emplace (m_nConnectionID, netBridge);
}

void CNetworkManager::SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge)
{
	std::shared_ptr<CUdpConnection> connection = std::make_shared<CUdpConnection> (_pkNetBridge, m_pkUdpSession);
	connection->Init ();
	_pkNetBridge->SetUdpConnection (connection);
}
