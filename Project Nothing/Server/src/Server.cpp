#include "stdafx.h"
#include "ConfigLoader.h"
#include "ServerGame.h"
#include "protocol/CommandProtocols.h"
#include "protocol/EventProtocols.h"
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
	Instance->InitGameLoopManager ();
	Instance->InitEntityManager ();
	Instance->InitEventManager ();
	Instance->InitProtocolManager ();
	Instance->InitNetworkManager ();
}

void CServer::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	CDBManager::Shutdown ();
	CGameManager::Shutdown ();
	CEntityManager::Shutdown ();
	CEventManager::Shutdown ();
	CProtocolManager::Shutdown ();
	CNetworkManager::Shutdown ();

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

void CServer::InitGameLoopManager ()
{
	unsigned short tickRate = CConfigLoader::GetConfig<unsigned short> ("game.tickrate");
	CServerGame::TickRate = tickRate;

	CGameManager::Init (m_kContext);
}

void CServer::InitEntityManager ()
{
	CEntityManager::Init ();
}

void CServer::InitEventManager ()
{
	CEventManager::Init (m_kContext);
	CEventManager::PushEvent (std::make_shared<CEventHeartbeat> ());
}

void CServer::InitProtocolManager ()
{
	CProtocolManager::Init ();

	CProtocolManager::RegisterNetEvent<NE_ServerLogin> (1, EProtocolType::Tcp);
	CProtocolManager::RegisterNetEvent<NE_ServerUdpConnect> (2, EProtocolType::Tcp);
	CProtocolManager::RegisterNetCommand<NC_ClientLoginResult> (51, EProtocolType::Tcp);
	CProtocolManager::RegisterNetCommand<NC_ClientUdpConnectResult> (52, EProtocolType::Udp);

	CProtocolManager::RegisterNetEvent<NE_ServerEcho> (100, EProtocolType::Tcp);
	CProtocolManager::RegisterNetEvent<NE_ServerCreateGame> (101, EProtocolType::Tcp);
	CProtocolManager::RegisterNetCommand<NC_ClientEchoResult> (200, EProtocolType::Tcp);
	CProtocolManager::RegisterNetCommand<NC_ClientCreateGameResult> (201, EProtocolType::Tcp);

	CProtocolManager::RegisterNetEvent<NE_ServerShutdown> (9000, EProtocolType::Tcp);
}

void CServer::InitNetworkManager ()
{
	std::string hostaddr = CConfigLoader::GetConfig<std::string> ("server.hostaddr");
	short tcpport = CConfigLoader::GetConfig<short> ("server.tcpport");
	short udpport = CConfigLoader::GetConfig<short> ("server.udpport");
	CNetworkManager::Init (m_kContext, hostaddr, tcpport, udpport);
}
