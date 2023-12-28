#pragma once

class CEvent
{
public:
	CEvent ();
	virtual ~CEvent ();

	virtual void Excute () = 0;

	uint64_t GetTime () const { return m_nTime; }
	void SetTime (uint64_t _nTime) { m_nTime = _nTime; }

	void Cancel () { m_bIsValid = false; }
	bool IsValid () const { return m_bIsValid; }

protected:
	uint64_t m_nTime;
	bool m_bIsValid;
};
