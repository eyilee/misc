#pragma once
#include "logger/Logger.h"
#include "framework/manager/BaseManager.h"

class INetProtocol;
class INetProtocolGenerator;
template<typename T>
class INetCommand;
template<typename T>
class INetEvent;
template<typename T>
class CNetProtocolGenerator;

class CProtocolManager : public CBaseManager<CProtocolManager>
{
public:
	CProtocolManager ();
	virtual ~CProtocolManager ();

	static void Init ();
	static void Shutdown ();

	template<typename T> requires std::is_class_v<INetCommand<T>>
	static void RegisterNetCommand (unsigned short _nID);

	template<typename T> requires std::is_class_v<INetEvent<T>>
	static void RegisterNetEvent (unsigned short _nID);

	static std::shared_ptr<INetProtocol> GenerateProtocol (unsigned short _nID);

private:
	template<typename T> requires std::is_class_v<INetCommand<T>>
	void RegisterCommand (unsigned short _nID);

	template<typename T> requires std::is_class_v<INetEvent<T>>
	void RegisterEvent (unsigned short _nID);

	std::shared_ptr<INetProtocol> Generate (unsigned short _nID);

private:
	std::map<int, std::shared_ptr<INetProtocolGenerator>> m_kProtocolMap;
};

template<typename T> requires std::is_class_v<INetCommand<T>>
inline static void CProtocolManager::RegisterNetCommand (unsigned short _nID)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->RegisterCommand<T> (_nID);
}

template<typename T> requires std::is_class_v<INetEvent<T>>
inline static void CProtocolManager::RegisterNetEvent (unsigned short _nID)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->RegisterEvent<T> (_nID);
}

template<typename T> requires std::is_class_v<INetCommand<T>>
inline void CProtocolManager::RegisterCommand (unsigned short _nID)
{
	INetCommand<T>::SetID (_nID);

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, std::static_pointer_cast<INetProtocolGenerator> (std::make_shared<CNetProtocolGenerator<T>> ()));
	}
	else {
		LOG_ERROR ("ID(%hu) has registered.", _nID);
	}
}

template<typename T> requires std::is_class_v<INetEvent<T>>
inline void CProtocolManager::RegisterEvent (unsigned short _nID)
{
	INetEvent<T>::SetID (_nID);

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, std::static_pointer_cast<INetProtocolGenerator> (std::make_shared<CNetProtocolGenerator<T>> ()));
	}
	else {
		LOG_ERROR ("ID(%hu) has registered.", _nID);
	}
}