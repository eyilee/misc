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

	long long get_time () const { return m_nTime; }
	void set_time (long long& _nTime) { m_nTime = _nTime; }

	void cancel () { m_bIsValid = false; }
	bool is_valid () { return m_bIsValid; }

protected:
	long long m_nTime;
	bool m_bIsValid;
};
