#pragma once
#include "framework/manager/BaseManager.h"

class CEvent;

class CEventManager : public CBaseManager<CEventManager>
{
public:
	CEventManager ();
	virtual ~CEventManager ();

	static void Init (boost::asio::io_context& _rkContext, const unsigned short& _rnTickRate);
	static void Shutdown ();

	static void PushEvent (std::shared_ptr<CEvent> _pkEvent);

private:
	void Run (boost::asio::io_context& _rkContext, const unsigned short& _rnEventRate);
	void Stop ();
	void Tick ();

	void Queue (std::shared_ptr<CEvent> _pkEvent);
	void Flush ();

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

private:
	bool m_bIsRunning;
	boost::posix_time::milliseconds m_kInterval;
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;

	std::forward_list<std::shared_ptr<CEvent>> m_kEventList;
	std::forward_list<std::shared_ptr<CEvent>> m_kEventQueue;
};
