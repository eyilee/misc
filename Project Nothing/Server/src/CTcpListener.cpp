#include "stdafx.h"

#include "CTcpConnection.h"
#include "CTcpSession.h"
#include "CTcpListener.h"

CTcpListener::CTcpListener (asio::io_context& _kio_context, short _nPort)
	: m_kAcceptor (_kio_context, tcp::endpoint (tcp::v4 (), _nPort))
{
}

void CTcpListener::Init (std::map<int, std::shared_ptr<CTcpSession>>& _kSession_manager)
{
	do_accept (_kSession_manager);
}

void CTcpListener::do_accept (std::map<int, std::shared_ptr<CTcpSession>>& _kSession_manager)
{
	m_kAcceptor.async_accept (
		[&](std::error_code error, tcp::socket socket)
		{
			if (error) {
				std::cerr << error.message () << std::endl;
			}
			else
			{
				std::shared_ptr<CTcpConnection> kConnection = std::make_shared<CTcpConnection> (std::move (socket));

				kConnection->Init ();

				std::shared_ptr<CTcpSession> kSession = std::make_shared<CTcpSession> (kConnection);

				_kSession_manager.emplace (0, std::move (kSession));
			}

			do_accept (_kSession_manager);
		});
}