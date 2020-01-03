#pragma once

using asio::ip::tcp;

class CTcpSession;
class CTcpListener;

class CSessionManager : public std::enable_shared_from_this<CSessionManager>
{
public:
	CSessionManager ();
	virtual ~CSessionManager ();

	void init (asio::io_context& _kIo_context, const short _nPort);

	void add_session (std::shared_ptr<CTcpSession>& _pSession);

private:
	std::list<std::shared_ptr<CTcpSession>> m_kSession_list;
	std::map<int, std::shared_ptr<CTcpSession>> m_kSession_map;

	std::shared_ptr<CTcpListener> m_pListener;
};

