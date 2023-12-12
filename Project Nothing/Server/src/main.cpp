#include "stdafx.h"
#include "ConfigLoader.h"
#include "Server.h"

BOOL WINAPI CtrlHandler (DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		case CTRL_C_EVENT:
			if (CServer::Instance != nullptr) {
				CServer::Instance->Shutdown ();
			}
			return TRUE;
		default:
			return FALSE;
	}
}

int main (int argc, char* argv[])
{
	const HMENU hMenu = GetSystemMenu (GetConsoleWindow (), FALSE);
	EnableMenuItem (hMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);

	std::shared_ptr<CLogger> logger = std::make_shared<CLogger> ();
	logger->Init ();

	std::shared_ptr<CServer> server = std::make_shared<CServer> ();
	server->Init ();

	if (SetConsoleCtrlHandler (CtrlHandler, TRUE)) {
		server->Run ();
	}

	if (logger != nullptr)
	{
		logger->Shutdown ();
	}

	EnableMenuItem (hMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

	return 0;
}
