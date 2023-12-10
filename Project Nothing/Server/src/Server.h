#pragma once
#include "ConfigLoader.h"

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
	void init_event_manager ();
	void init_protocol_manager ();
	void init_session_manager ();

public:
	static std::shared_ptr<CServer> Instance;

private:
	template<typename T, typename ... FARGS, typename ... ARGS>
	void setup_manager (std::shared_ptr<T> _pkInstance, void (T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args);

	template<typename T, typename ... FARGS, typename ... ARGS>
	void shutdown_manager (std::shared_ptr<T> _pkInstance, void (T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args);

private:
	boost::asio::io_context m_kContext;
	boost::asio::io_context::work m_kWork;

	CConfigLoader m_kConfigLoader;
};

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::setup_manager (std::shared_ptr<T> _pkInstance, void(T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args)
{
	if (_pkInstance == nullptr) {
		_pkInstance = std::make_shared<T> ();
	}

	(*_pkInstance.*_pfnFunction)(std::forward<ARGS> (_Args) ...);
}

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::shutdown_manager (std::shared_ptr<T> _pkInstance, void(T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args)
{
	if (_pkInstance != nullptr) {
		(*_pkInstance.*_pfnFunction)(std::forward<ARGS> (_Args) ...);
	}
}
