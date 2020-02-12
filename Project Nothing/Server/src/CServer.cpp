#include "stdafx.h"

#include "CServer.h"

std::shared_ptr<CServer> CServer::m_Instance = nullptr;

CServer::CServer ()
{
}

CServer::~CServer ()
{
}

void CServer::init (const int _nPort)
{
	if (m_Instance == nullptr) {
		m_Instance = shared_from_this ();
	}

	setup_manager<CDataBaseManager> (m_pData_base_manager, &CDataBaseManager::init);
	setup_manager<CEntityManager> (m_pEntity_manager, &CEntityManager::init);
	setup_manager<CProtocolManager> (m_pProtocol_manager, &CProtocolManager::init);
	setup_manager<CSessionManager> (m_pSession_manager, &CSessionManager::init, m_kIo_context, _nPort);
}

void CServer::run ()
{
	m_kIo_context.run ();
}
