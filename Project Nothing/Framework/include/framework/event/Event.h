#pragma once

class CEvent : public std::enable_shared_from_this<CEvent>
{
public:
	CEvent ();
	virtual ~CEvent ();

	virtual void Excute () = 0;

	long long GetTime () const { return m_nTime; }
	void SetTime (long long& _rnTime) { m_nTime = _rnTime; }

	void Cancel () { m_bIsValid = false; }
	bool IsValid () { return m_bIsValid; }

protected:
	long long m_nTime;
	bool m_bIsValid;
};
