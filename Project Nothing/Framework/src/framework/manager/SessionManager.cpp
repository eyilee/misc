#include "stdafx.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/SessionManager.h"

CSessionManager::CSessionManager ()
	: m_pkListener (nullptr)
	, m_pkUdpSession (nullptr)
{
}

CSessionManager::~CSessionManager ()
{
}

void CSessionManager::Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, const short _nTcpPort, const short _nUdpPort)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CSessionManager> ();
	Instance->Run (_rkContext, _rkHostAddr, _nTcpPort, _nUdpPort);
}

void CSessionManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Stop ();
	Instance = nullptr;
}

void CSessionManager::PushSession (std::shared_ptr<CTcpSession> _pkTcpSession)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Queue (_pkTcpSession);
}

void CSessionManager::Run (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, const short _nTcpPort, const short _nUdpPort)
{
	m_pkListener = std::make_shared<CTcpListener> (_rkContext, _rkHostAddr, _nTcpPort);
	m_pkListener->Init ();

	m_pkUdpSession = std::make_shared<CUdpSession> (_rkContext, _rkHostAddr, _nUdpPort);
	m_pkUdpSession->Init ();
}

void CSessionManager::Stop ()
{
	m_pkUdpSession->Shutdown ();

	for (auto& session : m_pkSessionList) {
		session->Shutdown ();
	}

	m_pkListener->Shutdown ();
}

void CSessionManager::Queue (std::shared_ptr<CTcpSession> _pkTcpSession)
{
	m_pkSessionList.emplace_back (_pkTcpSession);
}
