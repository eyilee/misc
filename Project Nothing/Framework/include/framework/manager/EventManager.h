#pragma once

class CEvent;

class CEventManager : public CBaseManager<CEventManager>
{
public:
	CEventManager ();
	virtual ~CEventManager ();

	void init (boost::asio::io_context& _kIo_context);
	void shutdown ();

	void add_event (const std::shared_ptr<CEvent>& _pEvent);

private:
	std::function<void (const boost::system::error_code& _kError_code)> m_fnTick;

	void tick ();

private:
	std::shared_ptr<boost::asio::deadline_timer> m_pTimer;
	boost::posix_time::milliseconds m_kInterval;

	std::forward_list<std::shared_ptr<CEvent>> m_kEvent_list;
};
