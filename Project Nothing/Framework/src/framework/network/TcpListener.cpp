#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/SessionManager.h"
#include "framework/network/TcpListener.h"

CTcpListener::CTcpListener (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort)
	: m_kAcceptor (_rkContext, tcp::endpoint (boost::asio::ip::address::from_string (_rkHostAddr.c_str ()), _nPort))
	, m_bIsRunning (false)
{
}

CTcpListener::~CTcpListener ()
{
}

void CTcpListener::Init ()
{
	m_bIsRunning = true;

	AsyncAccept ();
}

void CTcpListener::Shutdown ()
{
	m_bIsRunning = false;

	m_kAcceptor.cancel ();
	m_kAcceptor.close ();
}

void CTcpListener::AsyncAccept ()
{
	auto self (shared_from_this ());
	m_kAcceptor.async_accept ([&, self](const boost::system::error_code& _rkErrorCode, tcp::socket _rkSocket)
		{
			if (!m_bIsRunning) {
				return;
			}

			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
			else
			{
				std::shared_ptr<CTcpSession> session = std::make_shared<CTcpSession> (_rkSocket);
				session->Init ();

				if (CSessionManager::Instance != nullptr) {
					CSessionManager::Instance->AddSession (session);
				}

				AsyncAccept ();
			}
		});
}