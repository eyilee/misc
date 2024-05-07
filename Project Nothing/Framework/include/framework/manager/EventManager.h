#pragma once

#include "framework/manager/BaseManager.h"

class IEvent;

class CEventManager : public CBaseManager<CEventManager>
{
public:
	CEventManager ();
	virtual ~CEventManager ();

	static void Init (boost::asio::io_context& _rkContext);
	static void Shutdown ();

	static void PushEvent (std::shared_ptr<IEvent> _pkEvent);

private:
	void Run (boost::asio::io_context& _rkContext);
	void Stop ();
	void Update ();

	void Queue (std::shared_ptr<IEvent> _pkEvent);
	void Flush ();

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

private:
	bool m_bIsRunning;
	boost::posix_time::seconds m_kInterval;
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;

	std::forward_list<std::shared_ptr<IEvent>> m_kEventList;
	std::forward_list<std::shared_ptr<IEvent>> m_kEventQueue;
};
