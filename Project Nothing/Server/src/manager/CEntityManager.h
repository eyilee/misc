#pragma once

class CEntity;

class CEntityManager : public CBaseManager<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	void init ();

	std::shared_ptr<CEntity> create_entity ();

private:
	std::map<int, std::shared_ptr<CEntity>> m_kEntity_map;
};