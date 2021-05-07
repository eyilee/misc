#include "stdafx.h"

#include "CConfigLoader.h"
#include "CServer.h"

BOOL WINAPI CtrlHandler (DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		case CTRL_C_EVENT:
			if (CServer::Instance != nullptr) {
				CServer::Instance->shutdown ();
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

	std::shared_ptr<CServer> pServer = std::make_shared<CServer> ();

	pServer->init ();

	if (SetConsoleCtrlHandler (CtrlHandler, TRUE)) {
		pServer->run ();
	}

	if (pServer != nullptr) {
		pServer = nullptr;
	}

	EnableMenuItem (hMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

	return 0;
}
