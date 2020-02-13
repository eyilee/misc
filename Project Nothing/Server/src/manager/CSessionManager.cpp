#include "stdafx.h"

#include "CSessionManager.h"

CSessionManager::CSessionManager ()
	: m_pListener (nullptr)
{
}

CSessionManager::~CSessionManager ()
{
}

void CSessionManager::init (boost::asio::io_context& _kIo_context, const short _nPort)
{
	auto self (shared_from_this ());
	m_pListener = std::make_shared<CTcpListener> (_kIo_context, _nPort);
	m_pListener->init (self);
}

void CSessionManager::add_session (std::shared_ptr<CTcpSession>& _kSession)
{
	m_kSession_list.emplace_back (_kSession);
}
