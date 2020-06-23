#pragma once

#include "libpq-fe.h"

#include "manager/CBaseManager.h"

class CDBManager : public CBaseManager<CDBManager>
{
public:
	CDBManager ();
	virtual ~CDBManager ();

	void init (const std::string& _kUser, const std::string& _kPassword, const std::string& _kDBname, const std::string& _kHostaddr);

	void test ();

private:
	PGconn* m_pPGconn;
};