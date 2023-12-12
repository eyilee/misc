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

void CServer::init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_kConfigLoader.load ();

	init_db_manager ();
	init_entity_manager ();
	init_event_manager ();
	init_protocol_manager ();
	init_session_manager ();
}

void CServer::run ()
{
	m_kContext.run ();
}

void CServer::shutdown ()
{
	shutdown_manager (CDBManager::Instance, &CDBManager::shutdown);
	shutdown_manager (CEntityManager::Instance, &CEntityManager::shutdown);
	shutdown_manager (CEventManager::Instance, &CEventManager::shutdown);
	shutdown_manager (CProtocolManager::Instance, &CProtocolManager::shutdown);
	shutdown_manager (CSessionManager::Instance, &CSessionManager::shutdown);

	m_kContext.stop ();
}

void CServer::init_db_manager ()
{
	std::string user = m_kConfigLoader.get_config<std::string> ("db.user");
	std::string password = m_kConfigLoader.get_config<std::string> ("db.password");
	std::string dbname = m_kConfigLoader.get_config<std::string> ("db.dbname");
	std::string hostaddr = m_kConfigLoader.get_config<std::string> ("db.hostaddr");
	setup_manager (CDBManager::Instance, &CDBManager::init, user, password, dbname, hostaddr);
}

void CServer::init_entity_manager ()
{
	setup_manager (CEntityManager::Instance, &CEntityManager::init);
}

void CServer::init_event_manager ()
{
	setup_manager (CEventManager::Instance, &CEventManager::init, m_kContext);

	if (CEventManager::Instance != nullptr) {
		CEventManager::Instance->add_event (std::make_shared<CEventHeartbeat> ());
	}
}

void CServer::init_protocol_manager ()
{
	setup_manager (CProtocolManager::Instance, &CProtocolManager::init);

	if (CProtocolManager::Instance != nullptr)
	{
		CProtocolManager::Instance->register_protocol<ServerLogin> (1);
		CProtocolManager::Instance->register_protocol<ServerShutdown> (2);
		CProtocolManager::Instance->register_protocol<ClientLoginResult> (50);

		CProtocolManager::Instance->register_protocol<ServerEcho> (100);
		CProtocolManager::Instance->register_protocol<ClientEcho> (200);
	}
}

void CServer::init_session_manager ()
{
	std::string hostaddr = m_kConfigLoader.get_config<std::string> ("server.hostaddr");
	short tcpport = m_kConfigLoader.get_config<short> ("server.tcpport");
	short udpport = m_kConfigLoader.get_config<short> ("server.udpport");
	setup_manager (CSessionManager::Instance, &CSessionManager::init, m_kContext, hostaddr, tcpport, udpport);
}
