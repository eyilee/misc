#include "stdafx.h"

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

	m_pPGconn = PQconnectdb (kConnect_db);

	if (PQstatus (m_pPGconn) == CONNECTION_OK)
	{
		PQsetnonblocking (m_pPGconn, 1);
		test ();
		std::cout << "DB init succeeded." << std::endl;
	}
	else {
		std::cout << "DB init failed." << std::endl;
	}
}

void CDBManager::test ()
{
	const char* kQuery = "CREATE TABLE character (id integer NOT NULL, passward text NOT NULL);";
	PGresult* pPGresult = PQexec (m_pPGconn, kQuery);

	if (pPGresult == nullptr)
	{
		std::cout << PQerrorMessage (m_pPGconn) << std::endl;
	}

	ExecStatusType nExec_status_type = PQresultStatus (pPGresult);
	if (nExec_status_type == ExecStatusType::PGRES_BAD_RESPONSE
		|| nExec_status_type == ExecStatusType::PGRES_NONFATAL_ERROR
		|| nExec_status_type == ExecStatusType::PGRES_FATAL_ERROR)
	{
		std::cout << PQresultErrorMessage (pPGresult) << std::endl;
	}

	PQclear (pPGresult);
}
