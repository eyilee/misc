#pragma once

using asio::ip::tcp;

class CTcpListener;
class CTcpSession;

class CSessionManager
{
public:
	CSessionManager ();
	virtual ~CSessionManager ();

	void Init (asio::io_context& _kIo_context, short _nPort);

	void AddSession (std::shared_ptr<CTcpSession>& _kSession);

private:
	std::list<std::shared_ptr<CTcpSession>> m_kSession_list;
	std::map<int, std::shared_ptr<CTcpSession>> m_kSession_map;

	CTcpListener* m_pListener;
};

