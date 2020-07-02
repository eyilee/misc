#pragma once

class CTcpListener;
class CTcpSession;

class CSessionManager : public CBaseManager<CSessionManager>
{
public:
	CSessionManager ();
	virtual ~CSessionManager ();

	void init (boost::asio::io_context& _kIo_context, const short _nPort);
	void shutdown ();

	void add_session (std::shared_ptr<CTcpSession>& _pSession);

private:
	std::shared_ptr<CTcpListener> m_pListener;

	std::list<std::shared_ptr<CTcpSession>> m_kSession_list;
};

