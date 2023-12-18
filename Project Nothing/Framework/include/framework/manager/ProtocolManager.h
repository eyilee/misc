#pragma once
#include "logger/Logger.h"
#include "framework/manager/BaseManager.h"

class INetProtocol;
class INetProtocolGenerator;
template<typename T>
class CNetCommand;
template<typename T>
class CNetEvent;
template<typename T>
class CNetProtocolGenerator;

class CProtocolManager : public CBaseManager<CProtocolManager>
{
public:
	CProtocolManager ();
	virtual ~CProtocolManager ();

	static void Init ();
	static void Shutdown ();

	template<typename T>
	static void RegisterNetCommand (unsigned short _nID) requires std::is_class_v<CNetCommand<T>>;

	template<typename T>
	static void RegisterNetEvent (unsigned short _nID) requires std::is_class_v<CNetEvent<T>>;

	static std::shared_ptr<INetProtocol> GenerateProtocol (unsigned short _nID);

private:
	template<typename T>
	void RegisterCommand (unsigned short _nID) requires std::is_class_v<CNetCommand<T>>;

	template<typename T>
	void RegisterEvent (unsigned short _nID) requires std::is_class_v<CNetEvent<T>>;

	std::shared_ptr<INetProtocol> Generate (unsigned short _nID);

private:
	std::map<int, std::shared_ptr<INetProtocolGenerator>> m_kProtocolMap;
};

template<typename T>
inline static void CProtocolManager::RegisterNetCommand (unsigned short _nID) requires std::is_class_v<CNetCommand<T>>
{
	if (Instance == nullptr) {
		return;
	}

	Instance->RegisterCommand<T> (_nID);
}

template<typename T>
inline static void CProtocolManager::RegisterNetEvent (unsigned short _nID) requires std::is_class_v<CNetEvent<T>>
{
	if (Instance == nullptr) {
		return;
	}

	Instance->RegisterEvent<T> (_nID);
}

template<typename T>
inline void CProtocolManager::RegisterCommand (unsigned short _nID) requires std::is_class_v<CNetCommand<T>>
{
	CNetCommand<T>::SetID (_nID);

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, std::static_pointer_cast<INetProtocolGenerator> (std::make_shared<CNetProtocolGenerator<T>> ()));
	}
	else {
		LOG_ERROR ("ID(%hu) has registered.", _nID);
	}
}

template<typename T>
inline void CProtocolManager::RegisterEvent (unsigned short _nID) requires std::is_class_v<CNetEvent<T>>
{
	CNetEvent<T>::SetID (_nID);

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, std::static_pointer_cast<INetProtocolGenerator> (std::make_shared<CNetProtocolGenerator<T>> ()));
	}
	else {
		LOG_ERROR ("ID(%hu) has registered.", _nID);
	}
}