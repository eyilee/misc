#include "stdafx.h"

#include "CTcpListener.h"
#include "CTcpSession.h"
#include "CSessionManager.h"

CSessionManager::CSessionManager ()
	: m_pListener (nullptr)
{
}

CSessionManager::~CSessionManager ()
{
}

void CSessionManager::Init (asio::io_context& _kIo_context, short _nPort)
{
	m_pListener = new CTcpListener (_kIo_context, _nPort);
	m_pListener->Init (*this);

	std::cout << "SessionManager init." << std::endl;
}

void CSessionManager::AddSession (std::shared_ptr<CTcpSession>& _kSession)
{
	m_kSession_list.emplace_back (_kSession->shared_from_this ());
}
