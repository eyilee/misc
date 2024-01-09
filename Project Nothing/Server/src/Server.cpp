#include "stdafx.h"
#include "ConfigLoader.h"
#include "event/EventHeartbeat.h"
#include "game/ServerGame.h"
#include "protocol/NetCommandProtocols.h"
#include "protocol/NetEventProtocols.h"
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

	Instance->InitProtocolManager ();
	Instance->InitNetEntityManager ();
	Instance->InitNetworkManager ();

	Instance->InitEventManager ();
	Instance->InitGameManager ();
}

void CServer::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	CGameManager::Shutdown ();
	CEventManager::Shutdown ();

	CNetworkManager::Shutdown ();
	CNetEntityManager::Shutdown ();
	CProtocolManager::Shutdown ();

	CDBManager::Shutdown ();

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

void CServer::InitGameManager ()
{
	CServerGame::ServerTickRate = CConfigLoader::GetConfig<unsigned short> ("game.server_tickrate");
	CServerGame::ClientTickRate = CConfigLoader::GetConfig<unsigned short> ("game.client_tickrate");
	CServerGame::TickStep = CServerGame::ClientTickRate / CServerGame::ServerTickRate;
	CServerGame::TickInterval = 1000 / CServerGame::ServerTickRate;

	CGameManager::Init (m_kContext);
}

void CServer::InitNetEntityManager ()
{
	CNetEntityManager::Init ();
}

void CServer::InitEventManager ()
{
	CEventManager::Init (m_kContext);
	CEventManager::PushEvent (std::make_shared<CEventHeartbeat> ());
}

void CServer::InitProtocolManager ()
{
	CProtocolManager::Init ();

	CProtocolManager::RegisterNetEvent<NE_ServerLogin> (1);
	CProtocolManager::RegisterNetEvent<NE_ServerUdpConnect> (2);
	CProtocolManager::RegisterNetCommand<NC_ClientLoginResult> (51);
	CProtocolManager::RegisterNetCommand<NC_ClientUdpConnectResult> (52);

	CProtocolManager::RegisterNetEvent<NE_ServerEcho> (100);
	CProtocolManager::RegisterNetEvent<NE_ServerCreateGame> (101);
	CProtocolManager::RegisterNetCommand<NC_ClientEchoResult> (200);
	CProtocolManager::RegisterNetCommand<NC_ClientCreateGameResult> (201);
	CProtocolManager::RegisterNetCommand<NC_ClientJoinGame> (202);
	CProtocolManager::RegisterNetCommand<NC_ClientGameSnapshot> (203);

	CProtocolManager::RegisterNetEvent<NE_ServerShutdown> (9000);
}

void CServer::InitNetworkManager ()
{
	std::string hostaddr = CConfigLoader::GetConfig<std::string> ("server.hostaddr");
	unsigned short port = CConfigLoader::GetConfig<unsigned short> ("server.port");
	CNetworkManager::Init (m_kContext, hostaddr, port);
}
