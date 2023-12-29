#pragma once

class CTime
{
public:
	CTime ();
	virtual ~CTime ();

	static uint64_t GetSecond ();
	static uint64_t GetMiliSecond ();

public:
	static uint64_t FrameTime;
};
