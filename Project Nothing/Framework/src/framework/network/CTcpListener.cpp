#include "stdafx.h"

#include "framework/network/CTcpSession.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CSessionManager.h"

#include "framework/network/CTcpListener.h"

CTcpListener::CTcpListener (boost::asio::io_context& _kIo_context, const short _nPort)
	: m_kAcceptor (_kIo_context, tcp::endpoint (tcp::v4 (), _nPort))
	, m_bIs_running (false)
{
}

CTcpListener::~CTcpListener ()
{
}

void CTcpListener::init ()
{
	m_bIs_running = true;

	async_accept ();
}

void CTcpListener::shutdown ()
{
	m_bIs_running = false;

	m_kAcceptor.cancel ();
	m_kAcceptor.close ();
}

void CTcpListener::async_accept ()
{
	m_kAcceptor.async_accept ([&](const boost::system::error_code& _kError_code, tcp::socket _kSocket)
		{
			if (_kError_code && _kError_code != boost::asio::error::operation_aborted) {
				LOG_ERROR (_kError_code.message ());
			}
			else
			{
				std::shared_ptr<CTcpSession> pSession = std::make_shared<CTcpSession> (_kSocket);

				pSession->init ();

				CSessionManager::Instance->add_session (pSession);
			}

			if (m_bIs_running && _kError_code != boost::asio::error::operation_aborted) {
				async_accept ();
			}
		});
}