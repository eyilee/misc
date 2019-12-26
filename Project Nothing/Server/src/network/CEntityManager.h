#pragma once

class CEntity;

class CEntityManager : public std::enable_shared_from_this<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	std::shared_ptr<CEntity> create_entity ();

private:
	std::map<int, std::shared_ptr<CEntity>> m_kEntity_map;
};