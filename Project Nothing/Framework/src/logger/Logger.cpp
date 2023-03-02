#include "stdafx.h"
#include "logger/Logger.h"

using std::chrono::system_clock;

CLogger::CLogger ()
{
}

CLogger::~CLogger ()
{
}

std::shared_ptr<CLogger> CLogger::Instance = nullptr;

void CLogger::init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	if (!m_kFile_stream.is_open ())
	{
		std::time_t nTime = system_clock::to_time_t (system_clock::now ());
		struct tm kTime;
		localtime_s (&kTime, &nTime);

		char fileName[BUFSIZ] {};
		memset (fileName, 0, BUFSIZ);

		std::snprintf (&fileName[0], BUFSIZ, "%04d-%02d-%02d-%02d-%02d-%02d.log",
			kTime.tm_year + 1900,
			kTime.tm_mon + 1,
			kTime.tm_mday,
			kTime.tm_hour,
			kTime.tm_min,
			kTime.tm_sec);

		m_kFile_stream.open (fileName, std::ofstream::out | std::ofstream::app);
	}
}

void CLogger::shutdown ()
{
	if (m_kFile_stream.is_open ()) {
		m_kFile_stream.close ();
	}

	Instance = nullptr;
}

void CLogger::write (const char* _pString)
{
	if (m_kFile_stream.is_open ()) {
		m_kFile_stream << _pString << std::endl;
	}
}
