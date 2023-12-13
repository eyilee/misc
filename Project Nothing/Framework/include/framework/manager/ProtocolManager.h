#pragma once
#include "framework/manager/BaseManager.h"

class INetProtocol;
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

	template<typename T>
	static void RegisterProtocol (unsigned short _nID);

	static std::shared_ptr<INetProtocol> GenerateProtocol (unsigned short _nID);

private:
	template<typename T>
	void Register (unsigned short _nID);

	std::shared_ptr<INetProtocol> Generate (unsigned short _nID);

private:
	std::map<int, std::shared_ptr<INetProtocolGenerator>> m_kProtocolMap;
};

template<typename T>
inline static void CProtocolManager::RegisterProtocol (unsigned short _nID)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Register<T> (_nID);
}

template<typename T>
inline void CProtocolManager::Register (unsigned short _nID)
{
	T::SetID (_nID);

	std::shared_ptr<CNetProtocolGenerator<T>> generator = std::make_shared<CNetProtocolGenerator<T>> ();

	auto it = m_kProtocolMap.find (_nID);
	if (it == m_kProtocolMap.end ()) {
		m_kProtocolMap.emplace (_nID, generator);
	}
	else {
		it->second = generator;
	}
}
