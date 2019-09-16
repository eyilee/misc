#include "stdafx.h"

#include "CConnection.h"
#include "CListener.h"

CListener::CListener (asio::io_context& _kio_context, short _nPort)
	: m_kAcceptor (_kio_context, tcp::endpoint (tcp::v4 (), _nPort))
{
}

void CListener::Init (std::map<int, std::shared_ptr<CConnection>>& _kSession_manager)
{
	do_accept (_kSession_manager);
}

void CListener::do_accept (std::map<int, std::shared_ptr<CConnection>>& _kSession_manager)
{
	m_kAcceptor.async_accept (
		[&](std::error_code error, tcp::socket socket)
		{
			if (error) {
				std::cerr << error.message () << std::endl;
			}
			else
			{
				std::shared_ptr<CConnection> ptr = std::make_shared<CConnection> (std::move (socket));

				ptr->Init ();

				_kSession_manager.emplace (0, std::move (ptr));
			}

			do_accept (_kSession_manager);
		});
}