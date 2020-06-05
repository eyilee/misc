#include "stdafx.h"

#include "libpq-fe.h"

#include "CDBManager.h"

CDBManager::CDBManager ()
{
}

CDBManager::~CDBManager ()
{
}

void CDBManager::init (const std::string& _kUser, const std::string& _kPassword, const std::string& _kDBname, const std::string& _kHostaddr)
{
	char kConnect_db[256];
	std::memset (kConnect_db, 0, 256);
	sprintf_s (kConnect_db, "user=%s password=%s dbname=%s hostaddr=%s", _kUser.c_str (), _kPassword.c_str (), _kDBname.c_str (), _kHostaddr.c_str ());

	PGconn* conn = PQconnectdb (kConnect_db);

	if (PQstatus (conn) == CONNECTION_OK) {
		std::cout << "DB init succeeded." << std::endl;
	}
	else {
		std::cout << "DB init failed." << std::endl;
	}
}
