#pragma once

using boost::asio::ip::tcp;

class CTcpListener : public std::enable_shared_from_this<CTcpListener>
{
public:
	CTcpListener (boost::asio::io_context& _kIo_context, const short _nPort);
	virtual ~CTcpListener ();

	void init ();
	void shutdown ();

private:
	void async_accept ();

private:
	tcp::acceptor m_kAcceptor;

	bool m_bIs_running;
};