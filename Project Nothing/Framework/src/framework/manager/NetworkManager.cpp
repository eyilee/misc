#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/Random.h"
#include "framework/network/NetBridge.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpConnection.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpConnection.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/NetworkManager.h"

CNetworkManager::CNetworkManager (boost::asio::io_context& _rkContext)
	: m_rkContext (_rkContext)
	, m_nPort (0)
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

	Instance = std::make_shared<CNetworkManager> (_rkContext);
	Instance->Run (_rkHostAddr, _nPort);
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

void CNetworkManager::UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge, unsigned short _nPort)
{
	if (Instance == nullptr) {
		return;
	}

	std::shared_ptr<CUdpConnection> udpConnection = _pkNetBridge->GetUdpConnection ();
	if (udpConnection != nullptr) {
		return;
	}

	std::shared_ptr<CTcpConnection> tcpConnection = _pkNetBridge->GetTcpConnection ();
	if (tcpConnection == nullptr) {
		return;
	}

	tcp::endpoint tcpEndPoint = tcpConnection->GetRemoteEndpoint ();
	udp::endpoint udpEndPoint = udp::endpoint (tcpEndPoint.address (), _nPort);

	udp::socket socket (Instance->m_rkContext, udp::endpoint (boost::asio::ip::address::from_string (Instance->m_kHostAddr.c_str ()), 0));

	boost::system::error_code errorCode;
	socket.connect (udpEndPoint, errorCode);

	if (errorCode) {
		LOG_ERROR (errorCode.message ());
	}

	//socket.async_connect (udpEndPoint, [](const boost::system::error_code& _rkErrorCode)
	//	{
	//		if (_rkErrorCode) {
	//			LOG_ERROR (_rkErrorCode.message ());
	//		}
	//	});

	Instance->SetupUdpConnection (_pkNetBridge, socket);
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

void CNetworkManager::Run (const std::string& _rkHostAddr, unsigned short _nPort)
{
	m_kHostAddr = _rkHostAddr;
	m_nPort = _nPort;

	m_pkListener = std::make_shared<CTcpListener> (m_rkContext, m_kHostAddr, _nPort);
	m_pkListener->Init ();
}

void CNetworkManager::Stop ()
{
	m_pkListener->Shutdown ();

	for (auto& pair : m_kNetBridges) {
		pair.second->Shutdown ();
	}
}

std::shared_ptr<CNetBridge> CNetworkManager::CreateNetBridge ()
{
	uint32_t id = CRandom::GetValue<uint32_t> ();
	while (m_kNetBridges.find (id) != m_kNetBridges.end ()) {
		id = CRandom::GetValue<uint32_t> ();
	}

	std::shared_ptr<CNetBridge> netBridge = std::make_shared<CNetBridge> ();
	netBridge->SetID (id);
	m_kNetBridges.emplace (id, netBridge);
	return netBridge;
}

void CNetworkManager::SetupTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket)
{
	std::shared_ptr<CTcpConnection> connection = std::make_shared<CTcpConnection> (_pkNetBridge, std::make_shared<CTcpSession> (_rkSocket));
	connection->Init ();
	_pkNetBridge->SetTcpConnection (connection);
}

void CNetworkManager::SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, udp::socket& _rkSocket)
{
	std::shared_ptr<CUdpConnection> connection = std::make_shared<CUdpConnection> (_pkNetBridge, std::make_shared<CUdpSession> (_rkSocket));
	connection->Init ();
	_pkNetBridge->SetUdpConnection (connection);
}
