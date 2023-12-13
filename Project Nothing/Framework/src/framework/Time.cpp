#include "stdafx.h"
#include "framework/Time.h"

CTime::CTime ()
{
}

CTime::~CTime ()
{
}

uint64_t CTime::GetSecond ()
{
	auto now = std::chrono::system_clock::now ();
	return std::chrono::duration_cast<std::chrono::seconds> (now.time_since_epoch ()).count ();
}

uint64_t CTime::GetMiliSecond ()
{
	auto now = std::chrono::system_clock::now ();
	return std::chrono::duration_cast<std::chrono::milliseconds> (now.time_since_epoch ()).count ();
}
