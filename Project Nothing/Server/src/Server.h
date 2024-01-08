#pragma once

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer ();
	virtual ~CServer ();

	static void Init ();
	static void Shutdown ();

	static void Run ();

private:
	void InitDBManager ();
	void InitGameManager ();
	void InitNetEntityManager ();
	void InitEventManager ();
	void InitProtocolManager ();
	void InitNetworkManager ();

protected:
	static std::shared_ptr<CServer> Instance;

private:
	boost::asio::io_context m_kContext;
	boost::asio::io_context::work m_kWork;
};
