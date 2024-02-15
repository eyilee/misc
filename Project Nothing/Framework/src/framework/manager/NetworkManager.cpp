#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/Random.h"
#include "framework/network/NetBridge.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpConnection.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/NetworkManager.h"

using boost::asio::ip::udp;

CNetworkManager::CNetworkManager (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort)
	: m_rkContext (_rkContext)
	, m_kHostAddr (_rkHostAddr)
	, m_nPort (_nPort)
	, m_pkListener (nullptr)
{
}

CNetworkManager::~CNetworkManager ()
{
}

void CNetworkManager::Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CNetworkManager> (_rkContext, _rkHostAddr, _nPort);
	Instance->Run ();
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

	std::shared_ptr<CNetBridge> netBridge = Instance->CreateNetBridge ();
	std::shared_ptr<CTcpConnection> connection = std::make_shared<CTcpConnection> (std::make_shared<CTcpSession> (_rkSocket));
	connection->SetNetBridge (netBridge);
	netBridge->SetConnection (connection);
	connection->Init ();
}

std::shared_ptr<CUdpSession> CNetworkManager::UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge, unsigned short _nPort)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	std::shared_ptr<CTcpConnection> connection = _pkNetBridge->GetConnection ();
	if (connection == nullptr) {
		return nullptr;
	}

	udp::socket socket (Instance->m_rkContext, udp::endpoint (boost::asio::ip::address::from_string (Instance->m_kHostAddr.c_str ()), 0));

	udp::endpoint udpEndPoint = udp::endpoint (connection->GetRemoteEndpoint ().address (), _nPort);
	boost::system::error_code errorCode;
	socket.connect (udpEndPoint, errorCode);

	if (errorCode)
	{
		LOG_ERROR (errorCode.message ());

		return nullptr;
	}

	return std::make_shared<CUdpSession> (socket);
}

std::shared_ptr<CNetBridge> CNetworkManager::GetNetBridge (uint32_t _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	auto it = Instance->m_kNetBridges.find (_nID);
	if (it != Instance->m_kNetBridges.end ()) {
		return it->second;
	}

	return nullptr;
}

void CNetworkManager::RemoveNetBridge (uint32_t _nID)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->m_kNetBridges.erase (_nID);
}

void CNetworkManager::Run ()
{
	m_pkListener = std::make_shared<CTcpListener> (m_rkContext, m_kHostAddr, m_nPort);
	m_pkListener->Init ();
}

void CNetworkManager::Stop ()
{
	m_pkListener->Shutdown ();

	for (auto& [id, netBridge] : m_kNetBridges) {
		netBridge->Shutdown ();
	}
}

std::shared_ptr<CNetBridge> CNetworkManager::CreateNetBridge ()
{
	uint32_t id = 0;
	while (id == 0 || m_kNetBridges.find (id) != m_kNetBridges.end ()) {
		id = CRandom::GetValue<uint32_t> ();
	}

	std::shared_ptr<CNetBridge> netBridge = std::make_shared<CNetBridge> (id);
	m_kNetBridges.emplace (id, netBridge);
	return netBridge;
}
