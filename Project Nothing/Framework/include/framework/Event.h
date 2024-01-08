#pragma once

class IEvent
{
public:
	IEvent ();
	virtual ~IEvent ();

	virtual void Excute () = 0;

	inline uint64_t GetTime () const { return m_nTime; }
	inline void SetTime (uint64_t _nTime) { m_nTime = _nTime; }

	inline void Cancel () { m_bIsValid = false; }
	inline bool IsValid () const { return m_bIsValid; }

protected:
	uint64_t m_nTime;
	bool m_bIsValid;
};
