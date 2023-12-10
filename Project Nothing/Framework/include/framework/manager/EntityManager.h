#pragma once
#include "framework/manager/BaseManager.h"

class IEntity;
template<typename T>
class CEntity;

class CEntityManager : public CBaseManager<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	void init ();
	void shutdown ();

	template<typename T>
	std::shared_ptr<T> create_entity (int _nID);
	template<typename T>
	std::shared_ptr<T> get_entity (int _nID);

private:
	std::map<int, std::shared_ptr<IEntity>> m_kEntityMap;
};

template<typename T>
std::shared_ptr<T> CEntityManager::create_entity (int _nID)
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ())
	{
		std::shared_ptr<T> entity = std::make_shared<T> ();
		m_kEntityMap.emplace (_nID, entity);
	}

	return std::static_pointer_cast<T> (m_kEntityMap[_nID]);
}

template<typename T>
std::shared_ptr<T> CEntityManager::get_entity (int _nID)
{
	auto it = m_kEntityMap.find (_nID);
	if (it == m_kEntityMap.end ()) {
		return nullptr;
	}

	return std::static_pointer_cast<T> (*it);
}
