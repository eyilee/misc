#include "stdafx.h"
#include "ConfigLoader.h"
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
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CServer> ();
	Instance->InitDBManager ();
	Instance->InitEntityManager ();
	Instance->InitEventManager ();
	Instance->InitProtocolManager ();
	Instance->InitSessionManager ();
}

void CServer::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	CDBManager::Shutdown ();
	CEntityManager::Shutdown ();
	CEventManager::Shutdown ();
	CProtocolManager::Shutdown ();
	CSessionManager::Shutdown ();

	Instance->m_kContext.stop ();
}

void CServer::Run ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->m_kContext.run ();
}

void CServer::InitDBManager ()
{
	std::string user = CConfigLoader::GetConfig<std::string> ("db.user");
	std::string password = CConfigLoader::GetConfig<std::string> ("db.password");
	std::string dbname = CConfigLoader::GetConfig<std::string> ("db.dbname");
	std::string hostaddr = CConfigLoader::GetConfig<std::string> ("db.hostaddr");
	CDBManager::Init (user, password, dbname, hostaddr);
}

void CServer::InitEntityManager ()
{
	CEntityManager::Init ();
}

void CServer::InitEventManager ()
{
	unsigned short eventRate = CConfigLoader::GetConfig<unsigned short> ("event.eventrate");
	CEventManager::Init (m_kContext, eventRate);
	CEventManager::PushEvent (std::make_shared<CEventHeartbeat> ());
}

void CServer::InitProtocolManager ()
{
	CProtocolManager::Init ();

	CProtocolManager::RegisterProtocol<NE_ServerLogin> (1);
	CProtocolManager::RegisterProtocol<NE_ServerUdpConnect> (2);
	CProtocolManager::RegisterProtocol<NC_ClientLoginResult> (51);
	CProtocolManager::RegisterProtocol<NC_ClientUdpConnectResult> (52);

	CProtocolManager::RegisterProtocol<NE_ServerEcho> (100);
	CProtocolManager::RegisterProtocol<NC_ClientEchoResult> (200);

	CProtocolManager::RegisterProtocol<NE_ServerShutdown> (9000);
}

void CServer::InitSessionManager ()
{
	std::string hostaddr = CConfigLoader::GetConfig<std::string> ("server.hostaddr");
	short tcpport = CConfigLoader::GetConfig<short> ("server.tcpport");
	short udpport = CConfigLoader::GetConfig<short> ("server.udpport");
	CSessionManager::Init (m_kContext, hostaddr, tcpport, udpport);
}
