#include "stdafx.h"

#include "network/CSessionManager.h"
#include "network/CEntityManager.h"
#include "network/CProtocolManager.h"
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

	if (m_pSession_manager == nullptr)
	{
		m_pSession_manager = std::make_shared<CSessionManager> ();
		m_pSession_manager->init (m_kIo_context, _nPort);
	}

	if (m_pEntity_manager == nullptr) {
		m_pEntity_manager = std::make_shared<CEntityManager> ();
	}

	if (m_pProtocl_manager == nullptr)
	{
		m_pProtocl_manager = std::make_shared<CProtocolManager> ();
		m_pProtocl_manager->init ();
	}
}

void CServer::run ()
{
	m_kIo_context.run ();
}
