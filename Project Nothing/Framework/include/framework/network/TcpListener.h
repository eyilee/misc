#pragma once

using boost::asio::ip::tcp;

class CTcpListener : public std::enable_shared_from_this<CTcpListener>
{
public:
	CTcpListener (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort);
	virtual ~CTcpListener ();

	void Init ();
	void Shutdown ();

private:
	void AsyncAccept ();

private:
	tcp::acceptor m_kAcceptor;

	bool m_bIsRunning;
};