#pragma once

using boost::asio::ip::tcp;

class CSessionManager;

class CTcpListener
{
public:
	CTcpListener (boost::asio::io_context& _kIo_context, const short _nPort);
	virtual ~CTcpListener ();

	void init ();

private:
	void async_accept ();

private:
	tcp::acceptor m_kAcceptor;

	std::shared_ptr<CSessionManager> m_pSession_manager;
};