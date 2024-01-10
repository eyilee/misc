#pragma once
#include "framework/manager/BaseManager.h"

class INetEntity;

class CNetEntityManager : public CBaseManager<CNetEntityManager>
{
public:
	CNetEntityManager ();
	virtual ~CNetEntityManager ();

	static void Init ();
	static void Shutdown ();

	static std::shared_ptr<INetEntity> GetNetEntity (uint32_t _nID);

	template<typename T> requires std::is_base_of_v<INetEntity, T>
	static std::shared_ptr<INetEntity> GetOrCreateNetEntity (uint32_t _nID);

private:
	std::shared_ptr<INetEntity> Get (uint32_t _nID);

	template<typename T> requires std::is_base_of_v<INetEntity, T>
	std::shared_ptr<INetEntity> GetOrCreate (uint32_t _nID);


private:
	std::map<int, std::shared_ptr<INetEntity>> m_kNetEntityMap;
};

template<typename T> requires std::is_base_of_v<INetEntity, T>
inline std::shared_ptr<INetEntity> CNetEntityManager::GetOrCreateNetEntity (uint32_t _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->GetOrCreate<T> (_nID);
}

template<typename T> requires std::is_base_of_v<INetEntity, T>
inline std::shared_ptr<INetEntity> CNetEntityManager::GetOrCreate (uint32_t _nID)
{
	auto it = m_kNetEntityMap.find (_nID);
	if (it == m_kNetEntityMap.end ())
	{
		std::shared_ptr<T> netEntity = std::make_shared<T> ();
		netEntity->SetID (_nID);
		m_kNetEntityMap.emplace (_nID, netEntity);
		return netEntity;
	}
	else {
		return it->second;
	}
}
