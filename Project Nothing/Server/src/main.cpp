#include "stdafx.h"

#include "network/CSessionManager.h"
#include "network/CEntityManager.h"

static std::shared_ptr<CSessionManager> g_pSession_manager;
static std::shared_ptr<CEntityManager> g_pEntity_manager;

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: server <port>" << std::endl;
		return 1;
	}

	asio::io_context io_context;

	g_pSession_manager = std::make_shared<CSessionManager> ();
	g_pSession_manager->init (io_context, std::atoi (argv[1]));

	g_pEntity_manager = std::make_shared<CEntityManager> ();

	io_context.run ();

	return 0;
}
