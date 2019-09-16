#pragma once

using asio::ip::tcp;

class CListener
{
public:
	CListener (asio::io_context& _kIo_context, short _nPort);

	void Init (std::map<int, std::shared_ptr<CConnection>>& _kSession_manager);

private:
	void do_accept (std::map<int, std::shared_ptr<CConnection>>& _kSession_manager);

private:
	tcp::acceptor m_kAcceptor;
};