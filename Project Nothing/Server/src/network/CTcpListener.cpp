#include "stdafx.h"

#include "CTcpSession.h"
#include "CSessionManager.h"
#include "CTcpListener.h"

CTcpListener::CTcpListener (asio::io_context& _kio_context, const short _nPort)
	: m_kAcceptor (_kio_context, tcp::endpoint (tcp::v4 (), _nPort))
{
}

CTcpListener::~CTcpListener ()
{
}

void CTcpListener::init (std::shared_ptr<CSessionManager>& _pSession_manager)
{
	m_pSession_manager = _pSession_manager;

	async_accept ();

	std::cout << "TcpListener init, waiting for connection." << std::endl;
}

void CTcpListener::async_accept ()
{
	m_kAcceptor.async_accept ([&](std::error_code error, tcp::socket socket)
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