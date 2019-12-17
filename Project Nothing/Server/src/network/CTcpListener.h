#pragma once

using asio::ip::tcp;

class CTcpSession;
class CSessionManager;

class CTcpListener
{
public:
	CTcpListener (asio::io_context& _kIo_context, short _nPort);
	virtual ~CTcpListener ();

	void Init (CSessionManager& _kSession_manager);

private:
	void AsyncAccept ();

private:
	tcp::acceptor m_kAcceptor;

	CSessionManager* m_pSession_manager;
};