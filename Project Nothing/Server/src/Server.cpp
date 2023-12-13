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
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CServer> ();
	Instance->LoadConfig ();
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

void CServer::LoadConfig ()
{
	m_kConfigLoader.Load ();
}

void CServer::InitDBManager ()
{
	std::string user = m_kConfigLoader.GetConfig<std::string> ("db.user");
	std::string password = m_kConfigLoader.GetConfig<std::string> ("db.password");
	std::string dbname = m_kConfigLoader.GetConfig<std::string> ("db.dbname");
	std::string hostaddr = m_kConfigLoader.GetConfig<std::string> ("db.hostaddr");
	CDBManager::Init (user, password, dbname, hostaddr);
}

void CServer::InitEntityManager ()
{
	CEntityManager::Init ();
}

void CServer::InitEventManager ()
{
	unsigned short eventRate = m_kConfigLoader.GetConfig<unsigned short> ("event.eventrate");
	CEventManager::Init (m_kContext, eventRate);
	CEventManager::PushEvent (std::make_shared<CEventHeartbeat> ());
}

void CServer::InitProtocolManager ()
{
	CProtocolManager::Init ();

	CProtocolManager::RegisterProtocol<ServerLogin> (1);
	CProtocolManager::RegisterProtocol<ServerShutdown> (2);
	CProtocolManager::RegisterProtocol<ClientLoginResult> (50);

	CProtocolManager::RegisterProtocol<ServerEcho> (100);
	CProtocolManager::RegisterProtocol<ClientEcho> (200);
}

void CServer::InitSessionManager ()
{
	std::string hostaddr = m_kConfigLoader.GetConfig<std::string> ("server.hostaddr");
	short tcpport = m_kConfigLoader.GetConfig<short> ("server.tcpport");
	short udpport = m_kConfigLoader.GetConfig<short> ("server.udpport");
	CSessionManager::Init (m_kContext, hostaddr, tcpport, udpport);
}
