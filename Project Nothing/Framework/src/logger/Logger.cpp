#include "stdafx.h"
#include "logger/Logger.h"

using std::chrono::system_clock;

std::shared_ptr<CLogger> CLogger::Instance = nullptr;

CLogger::CLogger ()
{
}

CLogger::~CLogger ()
{
}

void CLogger::Init ()
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CLogger> ();
	Instance->Open ();
}

void CLogger::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Close ();
	Instance = nullptr;
}

void CLogger::Open ()
{
	if (m_kFileStream.is_open ()) {
		return;
	}

	std::time_t now = system_clock::to_time_t (system_clock::now ());
	struct tm time;
	localtime_s (&time, &now);

	char fileName[BUFSIZ] {};
	memset (fileName, 0, BUFSIZ);

	std::snprintf (&fileName[0], BUFSIZ, "%04d-%02d-%02d-%02d-%02d-%02d.log",
		time.tm_year + 1900,
		time.tm_mon + 1,
		time.tm_mday,
		time.tm_hour,
		time.tm_min,
		time.tm_sec);

	m_kFileStream.open (fileName, std::ofstream::out | std::ofstream::app);
}

void CLogger::Close ()
{
	if (!m_kFileStream.is_open ()) {
		return;
	}

	m_kFileStream.close ();
}

void CLogger::Write (const char* _szMessage)
{
	if (!m_kFileStream.is_open ()) {
		return;
	}

	m_kFileStream << _szMessage << std::endl;

#ifdef _DEBUG
	std::cout << _szMessage << std::endl;
#endif
}
