#pragma once
#include "framework/network/Entity.h"
#include "framework/manager/BaseManager.h"

class IEntity;

class CEntityManager : public CBaseManager<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	void Init ();
	void Shutdown ();

	std::shared_ptr<IEntity> GetEntity (int _nID);

	template<typename T>
	std::shared_ptr<IEntity> GetOrCreateEntity (int _nID) requires std::is_base_of<IEntity, T>::value;

private:
	std::map<int, std::shared_ptr<IEntity>> m_kEntityMap;
};

template<typename T>
inline std::shared_ptr<IEntity> CEntityManager::GetOrCreateEntity (int _nID)	requires std::is_base_of<IEntity, T>::value
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		std::shared_ptr<T> entity = std::make_shared<T> ();
		entity->SetID (_nID);
		m_kEntityMap.emplace (_nID, entity);
		return entity;
	}
	else {
		return it->second;
	}
}
