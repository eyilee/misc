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
	std::ofstream m_kFileStream;
public:
	static std::shared_ptr<CLogger> Instance;
};

template<typename ...ARGS>
inline static void CLogger::log (const char* _pString, ARGS && ..._kArgs)
{
	if (Instance != nullptr)
	{
		char buffer[BUFSIZ] {};
		std::snprintf (&buffer[0], BUFSIZ, _pString, std::forward<ARGS> (_kArgs) ...);
		Instance->write (&buffer[0]);
	}
}

#define LOG(content, ...) CLogger::log (("[LOG][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + "] " + content).c_str (), __VA_ARGS__);
#define LOG_EVENT(content, ...) CLogger::log (("[EVENT][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + "] " + content).c_str (), __VA_ARGS__);
#define LOG_ERROR(content, ...) CLogger::log (("[ERROR][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + ":%d] " + content).c_str (), __LINE__, __VA_ARGS__);
#define LOG_DEBUG(content, ...) CLogger::log (("[DEBUG][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + ":%d] " + content).c_str (), __LINE__, __VA_ARGS__);
