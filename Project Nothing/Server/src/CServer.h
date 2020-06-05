#pragma once

class CConfigLoader;
class CDBManager;
class CEntityManager;
class CProtocolManager;
class CSessionManager;

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer ();
	virtual ~CServer ();

	void init ();
	void run ();

	static std::shared_ptr<CServer>& instance () { return m_Instance; }

	static std::shared_ptr<CDBManager>& get_db_manager () { return m_Instance->m_pDB_manager; }
	static std::shared_ptr<CEntityManager>& get_entity_manager () { return m_Instance->m_pEntity_manager; }
	static std::shared_ptr<CProtocolManager>& get_protocol_manager () { return m_Instance->m_pProtocol_manager; }
	static std::shared_ptr<CSessionManager>& get_session_manager () { return m_Instance->m_pSession_manager; }

private:
	template<typename T, typename ... FARGS, typename ... ARGS>
	void setup_manager (std::shared_ptr<T>& _pInstance, void (T::* _pFunction)(FARGS ...), ARGS&& ... _kArgs);

private:
	static std::shared_ptr<CServer> m_Instance;

	boost::asio::io_context m_kIo_context;

	std::shared_ptr<CDBManager> m_pDB_manager;
	std::shared_ptr<CEntityManager> m_pEntity_manager;
	std::shared_ptr<CProtocolManager> m_pProtocol_manager;
	std::shared_ptr<CSessionManager> m_pSession_manager;
};

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::setup_manager (std::shared_ptr<T>& _pInstance, void(T::* _pFunction)(FARGS ...), ARGS&& ... _kArgs)
{
	if (_pInstance == nullptr) {
		_pInstance = std::make_shared<T> ();
	}

	(*_pInstance.*_pFunction)(std::forward<ARGS> (_kArgs) ...);
}
