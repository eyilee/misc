#include "stdafx.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CLogManager.h"

CLogManager::CLogManager ()
{
}

CLogManager::~CLogManager ()
{
}

void CLogManager::init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	if (!m_kFileStream.is_open ())
	{
		SYSTEMTIME kSystem_time;
		GetLocalTime (&kSystem_time);

		char fileName[1024] {};
		std::snprintf (&fileName[0], 1024, "%04d%02d%02d%02d%02d%02d.log",
			kSystem_time.wYear,
			kSystem_time.wMonth,
			kSystem_time.wDay,
			kSystem_time.wHour,
			kSystem_time.wMinute,
			kSystem_time.wSecond);

		m_kFileStream.open (fileName, std::ofstream::out | std::ofstream::app);
	}
}

void CLogManager::shutdown ()
{
	if (m_kFileStream.is_open ()) {
		m_kFileStream.close ();
	}

	Instance = nullptr;
}

void CLogManager::log (const char* _pString)
{
	if (m_kFileStream.is_open ()) {
		m_kFileStream << _pString << std::endl;
	}
}
