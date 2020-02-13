#include "stdafx.h"

#include "libpq-fe.h"

#include "CDBManager.h"

CDBManager::CDBManager ()
{
}

CDBManager::~CDBManager ()
{
}

void CDBManager::init ()
{
	PGconn* conn = PQconnectdb ("user=x password=x dbname=x hostaddr=x");

	if (PQstatus (conn) != CONNECTION_OK) {
		std::cout << "DB init succeeded." << std::endl;
	}
	else {
		std::cout << "DB init failed." << std::endl;
	}
}
