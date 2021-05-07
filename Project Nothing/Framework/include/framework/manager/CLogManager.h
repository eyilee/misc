#pragma once

class CLogManager : public CBaseManager<CLogManager>
{
public:
	CLogManager ();
	virtual ~CLogManager ();

	void init ();
	void shutdown ();

	template<typename ... ARGS>
	void log (const char* _pString, ARGS&& ... _kArgs);

private:
	void log (const char* _pString);

private:
	std::ofstream m_kFileStream;
};

template<typename ...ARGS>
inline void CLogManager::log (const char* _pString, ARGS && ..._kArgs)
{
	char buffer[1024] {};
	std::snprintf (&buffer[0], 1024, _pString, std::forward<ARGS> (_kArgs) ...);
	log (&buffer[0]);
}

#define LOG(content, ...) CLogManager::Instance->log (("[LOG][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + "] " + content).c_str (), __VA_ARGS__);
#define LOG_EVENT(content, ...) CLogManager::Instance->log (("[EVENT][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + "] " + content).c_str (), __VA_ARGS__);
#define LOG_ERROR(content, ...) CLogManager::Instance->log (("[ERROR][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + ":%d] " + content).c_str (), __LINE__, __VA_ARGS__);
#define LOG_DEBUG(content, ...) CLogManager::Instance->log (("[DEBUG][" + std::string (__TIME__) + "][" + std::string (__FUNCTION__) + ":%d] " + content).c_str (), __LINE__, __VA_ARGS__);
