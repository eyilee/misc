#pragma once

class CLogger
{
public:
	CLogger ();
	virtual ~CLogger ();

	static void Init ();
	static void Shutdown ();

	template<typename ... ARGS>
	static void Log (const char* _szMessage, ARGS&& ... _Args);

private:
	void Open ();
	void Close ();

	void Write (const char* _szMessage);

protected:
	static std::shared_ptr<CLogger> Instance;

private:
	std::ofstream m_kFileStream;
};

template<typename ...ARGS>
inline static void CLogger::Log (const char* _szMessage, ARGS&& ... _Args)
{
	if (Instance == nullptr) {
		return;
	}

	std::time_t now = std::chrono::system_clock::to_time_t (std::chrono::system_clock::now ());
	struct tm time;
	localtime_s (&time, &now);

	char buffer[BUFSIZ] {};
	std::snprintf (&buffer[0], BUFSIZ, _szMessage, time.tm_hour, time.tm_min, time.tm_sec, std::forward<ARGS> (_Args) ...);
	Instance->Write (&buffer[0]);
}

#define LOG_INFO(message, ...) CLogger::Log ((std::string ("%02d:%02d:%02d [INFO] ") + message).c_str (), __VA_ARGS__);
#define LOG_ERROR(message, ...) CLogger::Log ((std::string ("%02d:%02d:%02d [ERROR][") + std::string (__FUNCTION__) + ":%d] " + message).c_str (), __LINE__, __VA_ARGS__);
#define LOG_DEBUG(message, ...) CLogger::Log ((std::string ("%02d:%02d:%02d [DEBUG][") + std::string (__FUNCTION__) + ":%d] " + message).c_str (), __LINE__, __VA_ARGS__);
