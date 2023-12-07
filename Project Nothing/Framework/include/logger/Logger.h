#pragma once

using std::chrono::system_clock;

class CLogger : public std::enable_shared_from_this<CLogger>
{
public:
	CLogger ();
	virtual ~CLogger ();

	void init ();
	void shutdown ();

	template<typename ... ARGS>
	static void log (const char* _szMessage, ARGS&& ... _Args);

private:
	void write (const char* _szMessage);

private:
	std::ofstream m_kFileStream;

public:
	static std::shared_ptr<CLogger> Instance;
};

template<typename ...ARGS>
inline static void CLogger::log (const char* _szMessage, ARGS&& ... _Args)
{
	if (Instance == nullptr) {
		return;
	}

	std::time_t now = system_clock::to_time_t (system_clock::now ());
	struct tm time;
	localtime_s (&time, &now);

	char buffer[BUFSIZ] {};
	std::snprintf (&buffer[0], BUFSIZ, _szMessage, time.tm_hour, time.tm_min, time.tm_sec, std::forward<ARGS> (_Args) ...);
	Instance->write (&buffer[0]);
}

#define LOG_INFO(message, ...) CLogger::log ((std::string ("%02d:%02d:%02d [INFO] ") + message).c_str (), __VA_ARGS__);
#define LOG_ERROR(message, ...) CLogger::log ((std::string ("%02d:%02d:%02d [ERROR][") + std::string (__FUNCTION__) + ":%d] " + message).c_str (), __LINE__, __VA_ARGS__);
#define LOG_DEBUG(message, ...) CLogger::log ((std::string ("%02d:%02d:%02d [DEBUG][") + std::string (__FUNCTION__) + ":%d] " + message).c_str (), __LINE__, __VA_ARGS__);
