#pragma once
#include "framework/manager/BaseManager.h"

class CEvent;

class CEventManager : public CBaseManager<CEventManager>
{
public:
	CEventManager ();
	virtual ~CEventManager ();

	void init (boost::asio::io_context& _rkContext);
	void shutdown ();

	void add_event (std::shared_ptr<CEvent> _pkEvent);

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

	void tick ();

private:
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;
	boost::posix_time::milliseconds m_kInterval;

	std::forward_list<std::shared_ptr<CEvent>> m_kEventList;
};
