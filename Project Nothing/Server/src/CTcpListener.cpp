#include "stdafx.h"

#include "CTcpSession.h"
#include "CSessionManager.h"
#include "CTcpListener.h"

CTcpListener::CTcpListener (asio::io_context& _kio_context, short _nPort)
	: m_kAcceptor (_kio_context, tcp::endpoint (tcp::v4 (), _nPort))
	, m_pSession_manager (nullptr)
{
}

CTcpListener::~CTcpListener ()
{
}

void CTcpListener::Init (CSessionManager& _kSession_manager)
{
	m_pSession_manager = &_kSession_manager;

	AsyncAccept ();

	std::cout << "TcpListener init, waiting for connection." << std::endl;
}

void CTcpListener::AsyncAccept ()
{
	m_kAcceptor.async_accept ([&](std::error_code error, tcp::socket socket)
		{
			if (error) {
				std::cerr << error.message () << std::endl;
			}
			else
			{
				std::shared_ptr<CTcpSession> kSession = std::make_shared<CTcpSession> (std::move (socket));

				kSession->Init ();

				m_pSession_manager->AddSession (kSession);
			}

			AsyncAccept ();
		});
}