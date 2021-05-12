#pragma once

class IEvent
{
public:
	IEvent ();
	virtual ~IEvent ();
};

class CEvent : public IEvent, public std::enable_shared_from_this<CEvent>
{
public:
	CEvent ();
	virtual ~CEvent ();

	virtual void excute () {};

	unsigned long long get_time () const { return m_nTime; }
	void set_time (const unsigned long long& _nTime) { m_nTime = _nTime; }

	void cancel () { m_bIs_valid = false; }
	bool is_valid () { return m_bIs_valid; }

protected:
	unsigned long long m_nTime;
	bool m_bIs_valid;
};
