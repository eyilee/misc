#include "stdafx.h"

#include "CConfigLoader.h"
#include "CManagementTools.h"

std::shared_ptr<CManagementTools> CManagementTools::m_Instance = nullptr;

CManagementTools::CManagementTools ()
{
}

CManagementTools::~CManagementTools ()
{
}

void CManagementTools::init ()
{
	if (m_Instance == nullptr) {
		m_Instance = shared_from_this ();
	}

	CConfigLoader kConfig_loader;
	kConfig_loader.load ();

	if (m_pProtocol_manager == nullptr) {
		m_pProtocol_manager = std::make_shared<CProtocolManager> ();
	}

	m_pProtocol_manager->init ();

	if (m_pTcp_client == nullptr)
	{
		std::string kHost = kConfig_loader.get_config<std::string> ("server.host");
		std::string kPort = kConfig_loader.get_config<std::string> ("server.port");
		m_pTcp_client = std::make_shared<CTcpClient> (m_kIo_context, kHost, kPort);
	}

	m_pTcp_client->init ();
}

void CManagementTools::run ()
{
	m_kIo_context.run ();
}
