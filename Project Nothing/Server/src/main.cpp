#include "stdafx.h"
#include "ConfigLoader.h"
#include "Server.h"

BOOL WINAPI CtrlHandler (DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		case CTRL_C_EVENT:
			CServer::Shutdown ();
			return TRUE;
		default:
			return FALSE;
	}
}

int main (int argc, char* argv[])
{
	const HMENU hMenu = GetSystemMenu (GetConsoleWindow (), FALSE);
	EnableMenuItem (hMenu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);

	CLogger::Init ();
	CServer::Init ();

	if (SetConsoleCtrlHandler (CtrlHandler, TRUE)) {
		CServer::Run ();
	}

	CLogger::Shutdown ();

	EnableMenuItem (hMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

	return 0;
}
