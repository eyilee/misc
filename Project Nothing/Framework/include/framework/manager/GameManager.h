#pragma once
#include "framework/manager/BaseManager.h"

class IGameLoop;
class CEvent;

class CGameManager : public CBaseManager<CGameManager>
{
public:
	CGameManager ();
	virtual ~CGameManager ();

	static void Init (boost::asio::io_context& _rkContext);
	static void Shutdown ();

	static void AddGame (std::shared_ptr<IGameLoop> _pkGameLoop);

private:
	void Run (boost::asio::io_context& _rkContext);
	void Stop ();
	void Update ();

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

private:
	bool m_bIsRunning;
	boost::posix_time::milliseconds m_kInterval;
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;

	std::vector<std::shared_ptr<IGameLoop>> m_kGameLoops;
};
