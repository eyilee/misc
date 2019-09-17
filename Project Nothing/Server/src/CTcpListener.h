#pragma once

using asio::ip::tcp;

class CTcpListener
{
public:
	CTcpListener (asio::io_context& _kIo_context, short _nPort);

	void Init (std::map<int, std::shared_ptr<CTcpSession>>& _kSession_manager);

private:
	void do_accept (std::map<int, std::shared_ptr<CTcpSession>>& _kSession_manager);

private:
	tcp::acceptor m_kAcceptor;
};