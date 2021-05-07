#include "stdafx.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CDBManager.h"

CDBManager::CDBManager ()
	: m_pPGconn (nullptr)
{
}

CDBManager::~CDBManager ()
{
}

void CDBManager::init (const std::string& _kUser, const std::string& _kPassword, const std::string& _kDBname, const std::string& _kHostaddr)
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	char kConnect_db[BUFSIZ];
	ZeroMemory (kConnect_db, BUFSIZ);
	sprintf_s (kConnect_db, "user=%s password=%s dbname=%s hostaddr=%s", _kUser.c_str (), _kPassword.c_str (), _kDBname.c_str (), _kHostaddr.c_str ());

	m_pPGconn = PQconnectdb (kConnect_db);

	if (PQstatus (m_pPGconn) == CONNECTION_OK)
	{
		PQsetnonblocking (m_pPGconn, 1);
		test ();
		LOG_EVENT ("DB init succeeded.");
	}
	else {
		LOG_ERROR ("DB init failed.");
	}
}

void CDBManager::shutdown ()
{
	PQfinish (m_pPGconn);

	Instance = nullptr;
}

void CDBManager::test ()
{
	const char* kQuery = "CREATE TABLE character (id integer NOT NULL, passward text NOT NULL);";
	PGresult* pPGresult = PQexec (m_pPGconn, kQuery);

	if (pPGresult == nullptr) {
		LOG_ERROR (PQerrorMessage (m_pPGconn));
	}

	ExecStatusType nExec_status_type = PQresultStatus (pPGresult);
	if (nExec_status_type == ExecStatusType::PGRES_BAD_RESPONSE
		|| nExec_status_type == ExecStatusType::PGRES_NONFATAL_ERROR
		|| nExec_status_type == ExecStatusType::PGRES_FATAL_ERROR)
	{
		LOG_ERROR (PQresultErrorMessage (pPGresult));
	}

	PQclear (pPGresult);
}
