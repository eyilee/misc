#pragma once

class CConfigLoader;

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer ();
	virtual ~CServer ();

	void init ();

	void run ();
	void shutdown ();

	void init_db_manager ();
	void init_entity_manager ();
	void init_log_manager ();
	void init_protocol_manager ();
	void init_session_manager ();

public:
	static std::shared_ptr<CServer> Instance;

private:
	template<typename T, typename ... FARGS, typename ... ARGS>
	void setup_manager (std::shared_ptr<T>& _pInstance, void (T::* _pFunction)(FARGS ...), ARGS&& ... _kArgs);

	template<typename T, typename ... FARGS, typename ... ARGS>
	void shutdown_manager (std::shared_ptr<T>& _pInstance, void (T::* _pFunction)(FARGS ...), ARGS&& ... _kArgs);

private:
	boost::asio::io_context m_kIo_context;
	boost::asio::io_context::work m_kWork;

	CConfigLoader m_kConfig_loader;
};

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::setup_manager (std::shared_ptr<T>& _pInstance, void(T::* _pFunction)(FARGS ...), ARGS&& ... _kArgs)
{
	if (_pInstance == nullptr) {
		_pInstance = std::make_shared<T> ();
	}

	(*_pInstance.*_pFunction)(std::forward<ARGS> (_kArgs) ...);
}

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::shutdown_manager (std::shared_ptr<T>& _pInstance, void(T::* _pFunction)(FARGS...), ARGS && ..._kArgs)
{
	if (_pInstance != nullptr) {
		(*_pInstance.*_pFunction)(std::forward<ARGS> (_kArgs) ...);
	}
}
