#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/manager/DBManager.h"

CDBManager::CDBManager ()
	: m_pkPGconn (nullptr)
{
}

CDBManager::~CDBManager ()
{
}

void CDBManager::Init (const std::string& _rkUser, const std::string& _rkPassword, const std::string& _rkDBname, const std::string& _rkHostaddr)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CDBManager> ();
	Instance->Run (_rkUser, _rkPassword, _rkDBname, _rkHostaddr);
}

void CDBManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Stop ();
	Instance = nullptr;
}

void CDBManager::Run (const std::string& _rkUser, const std::string& _rkPassword, const std::string& _rkDBname, const std::string& _rkHostaddr)
{
	char connectDB[BUFSIZ];
	memset (connectDB, 0, BUFSIZ);
	sprintf_s (connectDB, "user=%s password=%s dbname=%s hostaddr=%s", _rkUser.c_str (), _rkPassword.c_str (), _rkDBname.c_str (), _rkHostaddr.c_str ());

	m_pkPGconn = PQconnectdb (connectDB);

	if (PQstatus (m_pkPGconn) == ConnStatusType::CONNECTION_OK)
	{
		PQsetnonblocking (m_pkPGconn, 1);
		Test ();
		LOG_INFO ("DB init succeeded.");
	}
	else {
		LOG_ERROR ("DB init failed.");
	}
}

void CDBManager::Stop ()
{
	PQfinish (m_pkPGconn);
}

void CDBManager::Test ()
{
	const char* query = "CREATE TABLE character (id integer NOT NULL, passward text NOT NULL);";
	PGresult* result = PQexec (m_pkPGconn, query);

	if (result == nullptr) {
		LOG_ERROR (PQerrorMessage (m_pkPGconn));
	}

	ExecStatusType statusType = PQresultStatus (result);
	if (statusType == ExecStatusType::PGRES_BAD_RESPONSE
		|| statusType == ExecStatusType::PGRES_NONFATAL_ERROR
		|| statusType == ExecStatusType::PGRES_FATAL_ERROR)
	{
		LOG_ERROR (PQresultErrorMessage (result));
	}

	PQclear (result);
}
