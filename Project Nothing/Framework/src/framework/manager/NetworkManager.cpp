#include "stdafx.h"
#include "framework/Random.h"
#include "framework/network/NetBridge.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpConnection.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpConnection.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/NetworkManager.h"

CNetworkManager::CNetworkManager ()
	: m_pkListener (nullptr)
	, m_pkUdpSession (nullptr)
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

	Instance->SetupTcpConnection (Instance->CreateNetBridge (), _rkSocket);
}

void CNetworkManager::UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->SetupUdpConnection (_pkNetBridge);
}

std::shared_ptr<CNetBridge> CNetworkManager::GetNetBridge (uint32_t _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	auto it = Instance->m_pkNetBridges.find (_nID);
	if (it != Instance->m_pkNetBridges.end ()) {
		return it->second;
	}

	return nullptr;
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

std::shared_ptr<CNetBridge> CNetworkManager::CreateNetBridge ()
{
	uint32_t id = CRandom::GetValue<uint32_t> ();
	while (m_pkNetBridges.find (id) != m_pkNetBridges.end ()) {
		id = CRandom::GetValue<uint32_t> ();
	}

	std::shared_ptr<CNetBridge> netBridge = std::make_shared<CNetBridge> ();
	netBridge->SetID (id);
	m_pkNetBridges.emplace (id, netBridge);
	return netBridge;
}

void CNetworkManager::SetupTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket)
{
	std::shared_ptr<CTcpConnection> connection = std::make_shared<CTcpConnection> (_pkNetBridge, std::make_shared<CTcpSession> (_rkSocket));
	connection->Init ();
	_pkNetBridge->SetTcpConnection (connection);
}

void CNetworkManager::SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge)
{
	std::shared_ptr<CUdpConnection> connection = std::make_shared<CUdpConnection> (_pkNetBridge, m_pkUdpSession);
	connection->Init ();
	_pkNetBridge->SetUdpConnection (connection);
}
