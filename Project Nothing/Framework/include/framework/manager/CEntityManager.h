#pragma once

class IEntity;
template <typename T>
class CEntity;

class CEntityManager : public CBaseManager<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	void init ();
	void shutdown ();

	template <typename T>
	std::shared_ptr<T> create_entity (int _nId);
	template <typename T>
	std::shared_ptr<T> get_entity (int _nId);

private:
	std::map<int, std::shared_ptr<IEntity>> m_kEntity_map;
};

template <typename T>
std::shared_ptr<T> CEntityManager::create_entity (int _nId)
{
	auto itEntity = m_kEntity_map.find (_nId);
	if (itEntity == m_kEntity_map.end ())
	{
		std::shared_ptr<T> pEntity = std::make_shared<T> ();
		m_kEntity_map.emplace (_nId, pEntity);
	}

	return std::static_pointer_cast<T> (m_kEntity_map[_nId]);
}

template <typename T>
std::shared_ptr<T> CEntityManager::get_entity (int _nId)
{
	auto itEntity = m_kEntity_map.find (_nId);
	if (itEntity == m_kEntity_map.end ()) {
		return nullptr;
	}

	return std::static_pointer_cast<T> (*itEntity);
}
