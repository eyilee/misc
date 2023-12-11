#include "stdafx.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/SessionManager.h"

CSessionManager::CSessionManager ()
	: m_pkListener (nullptr)
{
}

CSessionManager::~CSessionManager ()
{
}

void CSessionManager::init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, const short _nTcpPort, const short _nUdpPort)
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_pkListener = std::make_shared<CTcpListener> (_rkContext, _rkHostAddr, _nTcpPort);
	m_pkListener->init ();

	m_pkUdpSession = std::make_shared<CUdpSession> (_rkContext, _rkHostAddr, _nUdpPort);
	m_pkUdpSession->init ();
}

void CSessionManager::shutdown ()
{
	for (auto& session : m_pkSessionList) {
		session->shutdown ();
	}
	m_pkSessionList.clear ();

	m_pkListener->shutdown ();
	m_pkUdpSession->shutdown ();
}

void CSessionManager::add_session (std::shared_ptr<CTcpSession> _pkSession)
{
	m_pkSessionList.emplace_back (_pkSession);
}
