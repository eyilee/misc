#pragma once
#include "framework/manager/BaseManager.h"

class IEntity;

class CEntityManager : public CBaseManager<CEntityManager>
{
public:
	CEntityManager ();
	virtual ~CEntityManager ();

	void init ();
	void shutdown ();

	std::shared_ptr<IEntity> get_entity (int _nID);
	void set_entity (int _nID, std::shared_ptr<IEntity> _pkEntity);
	void remove_entity (int _nID);

private:
	std::map<int, std::shared_ptr<IEntity>> m_kEntityMap;
};
