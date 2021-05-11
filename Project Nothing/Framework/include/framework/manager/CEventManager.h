#pragma once

class CEventManager : public CBaseManager<CEventManager>
{
public:
	CEventManager ();
	virtual ~CEventManager ();

	void init (boost::asio::io_context& _kIo_context);
	void shutdown ();

private:
	std::function<void (const boost::system::error_code& _kError_code)> m_fnTick;

private:
	std::shared_ptr<boost::asio::deadline_timer> m_pTimer;
	boost::posix_time::milliseconds m_kInterval;
};
