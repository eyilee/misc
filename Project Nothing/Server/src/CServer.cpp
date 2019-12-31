#include "stdafx.h"

#include "network/CSessionManager.h"
#include "network/CEntityManager.h"
#include "CServer.h"

std::shared_ptr<CSessionManager> CServer::m_pSession_manager = nullptr;
std::shared_ptr<CEntityManager> CServer::m_pEntity_manager = nullptr;

CServer::CServer ()
{
}

CServer::~CServer ()
{
}

void CServer::Init (const int _nPort)
{
	if (m_pSession_manager == nullptr)
	{
		m_pSession_manager = std::make_shared<CSessionManager> ();
		m_pSession_manager->init (m_kIo_context, _nPort);
	}

	if (m_pEntity_manager == nullptr) {
		m_pEntity_manager = std::make_shared<CEntityManager> ();
	}
}

void CServer::Run ()
{
	m_kIo_context.run ();
}
