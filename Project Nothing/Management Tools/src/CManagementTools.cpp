#include "stdafx.h"

#include "CConfigLoader.h"
#include "CManagementTools.h"

std::shared_ptr<CManagementTools> CManagementTools::Instance = nullptr;

CManagementTools::CManagementTools ()
{
}

CManagementTools::~CManagementTools ()
{
}

void CManagementTools::init ()
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_kConfig_loader.load ();

	init_protocol_manager ();
	init_tcp_client ();
}

void CManagementTools::init_tcp_client ()
{
	if (m_pTcp_client == nullptr)
	{
		std::string kHost = m_kConfig_loader.get_config<std::string> ("server.host");
		std::string kPort = m_kConfig_loader.get_config<std::string> ("server.port");
		m_pTcp_client = std::make_shared<CTcpClient> (m_kIo_context, kHost, kPort);
	}

	m_pTcp_client->init ();
}

void CManagementTools::init_protocol_manager ()
{
	if (CProtocolManager::Instance == nullptr) {
		CProtocolManager::Instance = std::make_shared<CProtocolManager> ();
	}

	CProtocolManager::Instance->init ();
}

void CManagementTools::run ()
{
	m_kIo_context.run ();
}
