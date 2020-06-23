#include "stdafx.h"

#include "framework/network/CTcpSession.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CSessionManager.h"

#include "framework/network/CTcpListener.h"

CTcpListener::CTcpListener (boost::asio::io_context& _kIo_context, const short _nPort)
	: m_kAcceptor (_kIo_context, tcp::endpoint (tcp::v4 (), _nPort))
{
}

CTcpListener::~CTcpListener ()
{
}

void CTcpListener::init ()
{
	m_pSession_manager = CSessionManager::Instance;

	async_accept ();
}

void CTcpListener::async_accept ()
{
	m_kAcceptor.async_accept ([&](boost::system::error_code error, tcp::socket socket)
		{
			if (error) {
				std::cerr << error.message () << std::endl;
			}
			else
			{
				std::shared_ptr<CTcpSession> pSession = std::make_shared<CTcpSession> (socket);

				pSession->init ();

				m_pSession_manager->add_session (pSession);
			}

			async_accept ();
		});
}