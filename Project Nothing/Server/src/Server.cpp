#include "stdafx.h"
#include "protocol/ClientProtocols.h"
#include "protocol/ServerProtocols.h"
#include "event/EventHeartbeat.h"
#include "Server.h"

std::shared_ptr<CServer> CServer::Instance = nullptr;

CServer::CServer ()
	: m_kWork (m_kContext)
{
}

CServer::~CServer ()
{
}

void CServer::Init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_kConfigLoader.Load ();

	InitDBManager ();
	InitEntityManager ();
	InitEventManager ();
	InitProtocolManager ();
	InitSessionManager ();
}

void CServer::Run ()
{
	m_kContext.run ();
}

void CServer::Shutdown ()
{
	ShutdownManager (CDBManager::Instance, &CDBManager::Shutdown);
	ShutdownManager (CEntityManager::Instance, &CEntityManager::Shutdown);
	ShutdownManager (CEventManager::Instance, &CEventManager::Shutdown);
	ShutdownManager (CProtocolManager::Instance, &CProtocolManager::Shutdown);
	ShutdownManager (CSessionManager::Instance, &CSessionManager::Shutdown);

	m_kContext.stop ();
}

void CServer::InitDBManager ()
{
	std::string user = m_kConfigLoader.GetConfig<std::string> ("db.user");
	std::string password = m_kConfigLoader.GetConfig<std::string> ("db.password");
	std::string dbname = m_kConfigLoader.GetConfig<std::string> ("db.dbname");
	std::string hostaddr = m_kConfigLoader.GetConfig<std::string> ("db.hostaddr");
	SetupManager (CDBManager::Instance, &CDBManager::Init, user, password, dbname, hostaddr);
}

void CServer::InitEntityManager ()
{
	SetupManager (CEntityManager::Instance, &CEntityManager::Init);
}

void CServer::InitEventManager ()
{
	SetupManager (CEventManager::Instance, &CEventManager::Init, m_kContext);

	if (CEventManager::Instance != nullptr) {
		CEventManager::Instance->AddEvent (std::make_shared<CEventHeartbeat> ());
	}
}

void CServer::InitProtocolManager ()
{
	SetupManager (CProtocolManager::Instance, &CProtocolManager::Init);

	if (CProtocolManager::Instance != nullptr)
	{
		CProtocolManager::Instance->RegisterProtocol<ServerLogin> (1);
		CProtocolManager::Instance->RegisterProtocol<ServerShutdown> (2);
		CProtocolManager::Instance->RegisterProtocol<ClientLoginResult> (50);

		CProtocolManager::Instance->RegisterProtocol<ServerEcho> (100);
		CProtocolManager::Instance->RegisterProtocol<ClientEcho> (200);
	}
}

void CServer::InitSessionManager ()
{
	std::string hostaddr = m_kConfigLoader.GetConfig<std::string> ("server.hostaddr");
	short tcpport = m_kConfigLoader.GetConfig<short> ("server.tcpport");
	short udpport = m_kConfigLoader.GetConfig<short> ("server.udpport");
	SetupManager (CSessionManager::Instance, &CSessionManager::Init, m_kContext, hostaddr, tcpport, udpport);
}
