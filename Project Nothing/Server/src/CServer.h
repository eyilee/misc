#pragma once

class CSessionManager;
class CEntityManager;
class CProtocolManager;

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer ();
	virtual ~CServer ();

	void init (const int _nPort);
	void run ();

	static const std::shared_ptr<CServer>& instance () { return m_Instance; }

	std::shared_ptr<CSessionManager>& get_session_manager () { return m_pSession_manager; }
	std::shared_ptr<CEntityManager>& get_entity_manager () { return m_pEntity_manager; }
	std::shared_ptr<CProtocolManager>& get_potocol_manager () { return m_pProtocl_manager; }

private:
	static std::shared_ptr<CServer> m_Instance;

	asio::io_context m_kIo_context;

	std::shared_ptr<CSessionManager> m_pSession_manager;
	std::shared_ptr<CEntityManager> m_pEntity_manager;
	std::shared_ptr<CProtocolManager> m_pProtocl_manager;
};