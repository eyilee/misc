#pragma once
#include "libpq-fe.h"
#include "framework/manager/BaseManager.h"

class CDBManager : public CBaseManager<CDBManager>
{
public:
	CDBManager ();
	virtual ~CDBManager ();

	void init (const std::string& _rkUser, const std::string& _rkPassword, const std::string& _rkDBname, const std::string& _rkHostaddr);
	void shutdown ();

	// TODO: remove
	void test ();

private:
	PGconn* m_pkPGconn;
};