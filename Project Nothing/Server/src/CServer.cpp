#include "stdafx.h"

#include "protocol/ClientProtocols.h"
#include "protocol/ServerProtocols.h"

#include "CConfigLoader.h"
#include "CServer.h"

std::shared_ptr<CServer> CServer::Instance = nullptr;

CServer::CServer ()
	: m_kWork (m_kIo_context)
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

	m_kConfig_loader.load ();

	init_db_manager ();
	init_entity_manager ();
	init_protocol_manager ();
	init_session_manager ();
}

void CServer::run ()
{
	m_kIo_context.run ();
}

void CServer::shutdown ()
{
	CSessionManager::Instance->shutdown ();

	m_kIo_context.stop ();
}

void CServer::init_db_manager ()
{
	std::string kUser = m_kConfig_loader.get_config<std::string> ("db.user");
	std::string kPassword = m_kConfig_loader.get_config<std::string> ("db.password");
	std::string kDBname = m_kConfig_loader.get_config<std::string> ("db.dbname");
	std::string kHostaddr = m_kConfig_loader.get_config<std::string> ("db.hostaddr");
	setup_manager<CDBManager> (CDBManager::Instance, &CDBManager::init, kUser, kPassword, kDBname, kHostaddr);
}

void CServer::init_entity_manager ()
{
	setup_manager<CEntityManager> (CEntityManager::Instance, &CEntityManager::init);
}

void CServer::init_protocol_manager ()
{
	setup_manager<CProtocolManager> (CProtocolManager::Instance, &CProtocolManager::init);

	CProtocolManager::Instance->register_protocol<ServerLogin> (1);
	CProtocolManager::Instance->register_protocol<ServerShutdown> (2);
	CProtocolManager::Instance->register_protocol<ServerEcho> (100);

	CProtocolManager::Instance->register_protocol<ClientEcho> (200);
}

void CServer::init_session_manager ()
{
	int nPort = m_kConfig_loader.get_config<int> ("server.port");
	setup_manager<CSessionManager> (CSessionManager::Instance, &CSessionManager::init, m_kIo_context, nPort);
}
