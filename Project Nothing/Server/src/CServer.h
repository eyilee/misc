#pragma once

class CSessionManager;
class CEntityManager;

class CServer
{
public:
	CServer ();
	virtual ~CServer ();

	void Init (const int _nPort);
	void Run ();

private:
	asio::io_context m_kIo_context;

	static std::shared_ptr<CSessionManager> m_pSession_manager;
	static std::shared_ptr<CEntityManager> m_pEntity_manager;
};