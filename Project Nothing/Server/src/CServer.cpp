#include "stdafx.h"

#include "CConfigLoader.h"
#include "CServer.h"

std::shared_ptr<CServer> CServer::m_Instance = nullptr;

CServer::CServer ()
{
}

CServer::~CServer ()
{
}

void CServer::init ()
{
	if (m_Instance == nullptr) {
		m_Instance = shared_from_this ();
	}

	CConfigLoader kConfig_loader;
	kConfig_loader.load ();

	std::string kUser = kConfig_loader.get_config<std::string> ("db.user");
	std::string kPassword = kConfig_loader.get_config<std::string> ("db.password");
	std::string kDBname = kConfig_loader.get_config<std::string> ("db.dbname");
	std::string kHostaddr = kConfig_loader.get_config<std::string> ("db.hostaddr");
	setup_manager<CDBManager> (m_pDB_manager, &CDBManager::init, kUser, kPassword, kDBname, kHostaddr);

	setup_manager<CEntityManager> (m_pEntity_manager, &CEntityManager::init);
	setup_manager<CProtocolManager> (m_pProtocol_manager, &CProtocolManager::init);

	int nPort = kConfig_loader.get_config<int> ("server.port");
	setup_manager<CSessionManager> (m_pSession_manager, &CSessionManager::init, m_kIo_context, nPort);
}

void CServer::run ()
{
	m_kIo_context.run ();
}
