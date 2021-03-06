#include "stdafx.h"

#include "framework/network/CTcpListener.h"
#include "framework/network/CTcpSession.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CSessionManager.h"

CSessionManager::CSessionManager ()
	: m_pListener (nullptr)
{
}

CSessionManager::~CSessionManager ()
{
}

void CSessionManager::init (boost::asio::io_context& _kIo_context, const short _nPort)
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_pListener = std::make_shared<CTcpListener> (_kIo_context, _nPort);
	m_pListener->init ();
}

void CSessionManager::shutdown ()
{
	for (auto& pSession : m_kSession_list) {
		pSession->shutdown ();
	}
	m_kSession_list.clear ();

	m_pListener->shutdown ();
}

void CSessionManager::add_session (std::shared_ptr<CTcpSession>& _kSession)
{
	m_kSession_list.emplace_back (_kSession);
}
