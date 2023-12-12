#pragma once
#include "ConfigLoader.h"

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer ();
	virtual ~CServer ();

	void Init ();

	void Run ();
	void Shutdown ();

	void InitDBManager ();
	void InitEntityManager ();
	void InitEventManager ();
	void InitProtocolManager ();
	void InitSessionManager ();

public:
	static std::shared_ptr<CServer> Instance;

private:
	template<typename T, typename ... FARGS, typename ... ARGS>
	void SetupManager (std::shared_ptr<T> _pkInstance, void (T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args);

	template<typename T, typename ... FARGS, typename ... ARGS>
	void ShutdownManager (std::shared_ptr<T> _pkInstance, void (T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args);

private:
	boost::asio::io_context m_kContext;
	boost::asio::io_context::work m_kWork;

	CConfigLoader m_kConfigLoader;
};

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::SetupManager (std::shared_ptr<T> _pkInstance, void(T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args)
{
	if (_pkInstance == nullptr) {
		_pkInstance = std::make_shared<T> ();
	}

	(*_pkInstance.*_pfnFunction)(std::forward<ARGS> (_Args) ...);
}

template<typename T, typename ...FARGS, typename ...ARGS>
inline void CServer::ShutdownManager (std::shared_ptr<T> _pkInstance, void(T::* _pfnFunction)(FARGS ...), ARGS&& ... _Args)
{
	if (_pkInstance != nullptr) {
		(*_pkInstance.*_pfnFunction)(std::forward<ARGS> (_Args) ...);
	}
}
