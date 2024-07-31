#pragma once

class CTcpListener : public std::enable_shared_from_this<CTcpListener>
{
public:
	CTcpListener (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort);
	virtual ~CTcpListener ();

	void Init ();
	void Shutdown ();

private:
	void AsyncAccept ();

private:
	boost::asio::ip::tcp::acceptor m_kAcceptor;

	bool m_bIsRunning;
};
