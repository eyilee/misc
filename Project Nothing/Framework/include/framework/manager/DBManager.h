#pragma once

#include "libpq-fe.h"
#include "framework/manager/BaseManager.h"

class CDBManager : public CBaseManager<CDBManager>
{
public:
	CDBManager ();
	virtual ~CDBManager ();

	static void Init (const std::string& _rkUser, const std::string& _rkPassword, const std::string& _rkDBname, const std::string& _rkHostaddr);
	static void Shutdown ();

private:
	void Run (const std::string& _rkUser, const std::string& _rkPassword, const std::string& _rkDBname, const std::string& _rkHostaddr);
	void Stop ();

	// TODO: remove
	void Test ();

private:
	PGconn* m_pkPGconn;
};
