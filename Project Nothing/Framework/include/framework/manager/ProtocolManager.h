#pragma once
#include "logger/Logger.h"
#include "framework/manager/BaseManager.h"
#include "framework/network/NetProtocol.h"

class INetProtocolGenerator;
template<typename T>
class CNetProtocolGenerator;

class CProtocolManager : public CBaseManager<CProtocolManager>
{
public:
	CProtocolManager ();
	virtual ~CProtocolManager ();

	static void Init ();
	static void Shutdown ();

	template<typename T> requires std::is_class_v<CNetCommand<T>>
	static void RegisterNetCommand (unsigned short _nID, EProtocolType _nType);

	template<typename T> requires std::is_class_v<CNetEvent<T>>
	static void RegisterNetEvent (unsigned short _nID, EProtocolType _nType);

	static std::shared_ptr<INetProtocol> GenerateProtocol (unsigned short _nID);

private:
	template<typename T> requires std::is_class_v<CNetCommand<T>>
	void RegisterCommand (unsigned short _nID, EProtocolType _nType);

	template<typename T> requires std::is_class_v<CNetEvent<T>>
	void RegisterEvent (unsigned short _nID, EProtocolType _nType);

	std::shared_ptr<INetProtocol> Generate (unsigned short _nID);

private:
	std::map<int, std::shared_ptr<INetProtocolGenerator>> m_kProtocolMap;
};

template<typename T> requires std::is_class_v<CNetCommand<T>>
inline static void CProtocolManager::RegisterNetCommand (unsigned short _nID, EProtocolType _nType)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->RegisterCommand<T> (_nID, _nType);
}

template<typename T> requires std::is_class_v<CNetEvent<T>>
inline static void CProtocolManager::RegisterNetEvent (unsigned short _nID, EProtocolType _nType)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->RegisterEvent<T> (_nID, _nType);
}

template<typename T> requires std::is_class_v<CNetCommand<T>>
inline void CProtocolManager::RegisterCommand (unsigned short _nID, EProtocolType _nType)
{
	CNetCommand<T>::SetID (_nID);
	CNetCommand<T>::SetType (_nType);

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, std::static_pointer_cast<INetProtocolGenerator> (std::make_shared<CNetProtocolGenerator<T>> ()));
	}
	else {
		LOG_ERROR ("ID(%hu) has registered.", _nID);
	}
}

template<typename T> requires std::is_class_v<CNetEvent<T>>
inline void CProtocolManager::RegisterEvent (unsigned short _nID, EProtocolType _nType)
{
	CNetEvent<T>::SetID (_nID);
	CNetEvent<T>::SetType (_nType);

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, std::static_pointer_cast<INetProtocolGenerator> (std::make_shared<CNetProtocolGenerator<T>> ()));
	}
	else {
		LOG_ERROR ("ID(%hu) has registered.", _nID);
	}
}