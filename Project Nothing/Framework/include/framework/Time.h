#pragma once

class CTime
{
public:
	CTime ();
	virtual ~CTime ();

	static uint64_t GetSecond ();
	static uint64_t GetMiliSecond ();

private:
	uint64_t m_nMiliSeconds;
};
