#pragma once
#include "framework/manager/BaseManager.h"

class CEvent;

class CEventManager : public CBaseManager<CEventManager>
{
public:
	CEventManager ();
	virtual ~CEventManager ();

	void Init (boost::asio::io_context& _rkContext);
	void Shutdown ();

	void AddEvent (std::shared_ptr<CEvent> _pkEvent);

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

	void Tick ();

private:
	bool m_bIsRunning;
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;
	boost::posix_time::milliseconds m_kInterval;

	std::forward_list<std::shared_ptr<CEvent>> m_kEventList;
};
