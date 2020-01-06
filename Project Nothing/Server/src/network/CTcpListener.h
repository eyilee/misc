#pragma once

using asio::ip::tcp;

class CSessionManager;

class CTcpListener
{
public:
	CTcpListener (asio::io_context& _kIo_context, const short _nPort);
	virtual ~CTcpListener ();

	void init (std::shared_ptr<CSessionManager>& _pSession_manager);

private:
	void async_accept ();

private:
	tcp::acceptor m_kAcceptor;

	std::shared_ptr<CSessionManager> m_pSession_manager;
};