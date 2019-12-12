#include "stdafx.h"

#include "CTcpSession.h"
#include "CTcpListener.h"

CTcpListener::CTcpListener (asio::io_context& _kio_context, short _nPort)
	: m_kAcceptor (_kio_context, tcp::endpoint (tcp::v4 (), _nPort))
{
}

void CTcpListener::Init (std::map<int, std::shared_ptr<CTcpSession>>& _kSession_manager)
{
	AsyncAccept (_kSession_manager);
}

void CTcpListener::AsyncAccept (std::map<int, std::shared_ptr<CTcpSession>>& _kSession_manager)
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

				_kSession_manager.emplace (0, std::move (kSession));
			}

			AsyncAccept (_kSession_manager);
		});
}