#pragma once
#include "framework/manager/BaseManager.h"

class CEvent;

class CGameLoopManager : public CBaseManager<CGameLoopManager>
{
public:
	CGameLoopManager ();
	virtual ~CGameLoopManager ();

	static void Init (boost::asio::io_context& _rkContext);
	static void Shutdown ();

private:
	void Run (boost::asio::io_context& _rkContext);
	void Stop ();
	void Tick ();

	// TODO: add game loop entities

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

private:
	bool m_bIsRunning;
	boost::posix_time::milliseconds m_kInterval;
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;
};
