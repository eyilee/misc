#pragma once

class CLogger : public std::enable_shared_from_this<CLogger>
{
public:
	CLogger ();
	virtual ~CLogger ();

	void init ();
	void shutdown ();

	template<typename ... ARGS>
	static void log (const char* _pString, ARGS&& ... _kArgs);

private:
	void write (const char* _pString);

private:
	std::ofstream m_kFile_stream;
public:
	static std::shared_ptr<CLogger> Instance;
};

template<typename ...ARGS>
inline static void CLogger::log (const char* _pString, ARGS && ..._kArgs)
{
	if (Instance != nullptr)
	{
		SYSTEMTIME kSystem_time;
		GetLocalTime (&kSystem_time);

		char buffer[BUFSIZ] {};
		std::snprintf (&buffer[0], BUFSIZ, _pString, kSystem_time.wHour, kSystem_time.wMinute, kSystem_time.wSecond, std::forward<ARGS> (_kArgs) ...);
		Instance->write (&buffer[0]);
	}
}

#define LOG_INFO(content, ...) CLogger::log ((std::string ("%02hu:%02hu:%02hu [INFO] ") + content).c_str (), __VA_ARGS__);
#define LOG_ERROR(content, ...) CLogger::log ((std::string ("%02hu:%02hu:%02hu [ERROR][") + std::string (__FUNCTION__) + ":%d] " + content).c_str (), __LINE__, __VA_ARGS__);
#define LOG_DEBUG(content, ...) CLogger::log ((std::string ("%02hu:%02hu:%02hu [DEBUG][") + std::string (__FUNCTION__) + ":%d] " + content).c_str (), __LINE__, __VA_ARGS__);
