#pragma once
#include "framework/network/Entity.h"
#include "framework/manager/BaseManager.h"

class IEntity;

class CEntityManager : public CBaseManager<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	static void Init ();
	static void Shutdown ();

	static std::shared_ptr<IEntity> GetEntity (int _nID);

	template<typename T>
	static std::shared_ptr<IEntity> GetOrCreateEntity (int _nID) requires std::is_base_of_v<IEntity, T>;

private:
	std::shared_ptr<IEntity> Get (int _nID);

	template<typename T>
	std::shared_ptr<IEntity> GetOrCreate (int _nID) requires std::is_base_of_v<IEntity, T>;


private:
	std::map<int, std::shared_ptr<IEntity>> m_kEntityMap;
};

template<typename T>
inline std::shared_ptr<IEntity> CEntityManager::GetOrCreateEntity (int _nID) requires std::is_base_of_v<IEntity, T>
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->GetOrCreate<T> (_nID);
}

template<typename T>
inline std::shared_ptr<IEntity> CEntityManager::GetOrCreate (int _nID) requires std::is_base_of_v<IEntity, T>
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		std::shared_ptr<IEntity> entity = std::static_pointer_cast<IEntity> (std::make_shared<T> ());
		entity->SetID (_nID);
		m_kEntityMap.emplace (_nID, entity);
		return entity;
	}
	else {
		return it->second;
	}
}
