#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/NetworkManager.h"
#include "framework/network/TcpListener.h"

using boost::asio::ip::tcp;

CTcpListener::CTcpListener (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort)
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
}

void CTcpListener::AsyncAccept ()
{
	auto self (shared_from_this ());
	m_kAcceptor.async_accept ([this, self](const boost::system::error_code& _rkErrorCode, tcp::socket _rkSocket)
		{
			if (!m_bIsRunning)
			{
				m_kAcceptor.cancel ();
				m_kAcceptor.close ();
			}
			else
			{
				if (_rkErrorCode) {
					LOG_ERROR (_rkErrorCode.message ());
				}
				else
				{
					CNetworkManager::TcpAccept (_rkSocket);

					AsyncAccept ();
				}
			}
		});
}
