#pragma once

class CEntityManager;
class CProtocolManager;
class CSessionManager;

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer ();
	virtual ~CServer ();

	void init (const int _nPort);
	void run ();

	static const std::shared_ptr<CServer>& instance () { return m_Instance; }

	std::shared_ptr<CEntityManager>& get_entity_manager () { return m_pEntity_manager; }
	std::shared_ptr<CProtocolManager>& get_protocol_manager () { return m_pProtocol_manager; }
	std::shared_ptr<CSessionManager>& get_session_manager () { return m_pSession_manager; }

private:
	template<typename T, typename ... FARGS, typename ... ARGS>
	void setup_manager (std::shared_ptr<T>& instance, void (T::* function)(FARGS ...), ARGS&& ... args);

private:
	static std::shared_ptr<CServer> m_Instance;

	asio::io_context m_kIo_context;

	std::shared_ptr<CEntityManager> m_pEntity_manager;
	std::shared_ptr<CProtocolManager> m_pProtocol_manager;
	std::shared_ptr<CSessionManager> m_pSession_manager;
};

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::setup_manager (std::shared_ptr<T>& instance, void(T::* function)(FARGS...), ARGS&& ...args)
{
	if (instance == nullptr) {
		instance = std::make_shared<T> ();
	}

	(*instance.*function)(std::forward<ARGS> (args) ...);
}
