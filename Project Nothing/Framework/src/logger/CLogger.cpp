#include "stdafx.h"

#include "logger/CLogger.h"

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

	if (!m_kFileStream.is_open ())
	{
		SYSTEMTIME kSystem_time;
		GetLocalTime (&kSystem_time);

		char fileName[BUFSIZ] {};
		std::snprintf (&fileName[0], BUFSIZ, "%04d%02d%02d%02d%02d%02d.log",
			kSystem_time.wYear,
			kSystem_time.wMonth,
			kSystem_time.wDay,
			kSystem_time.wHour,
			kSystem_time.wMinute,
			kSystem_time.wSecond);

		m_kFileStream.open (fileName, std::ofstream::out | std::ofstream::app);
	}
}

void CLogger::shutdown ()
{
	if (m_kFileStream.is_open ()) {
		m_kFileStream.close ();
	}

	Instance = nullptr;
}

void CLogger::write (const char* _pString)
{
	if (m_kFileStream.is_open ()) {
		m_kFileStream << _pString << std::endl;
	}
}
