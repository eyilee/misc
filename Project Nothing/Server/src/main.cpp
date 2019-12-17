#include "stdafx.h"

#include "CSessionManager.h"

static CSessionManager* g_kSession_manager;

int main (int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: server <port>" << std::endl;
			return 1;
		}

		asio::io_context io_context;

		g_kSession_manager = new CSessionManager ();
		g_kSession_manager->Init (io_context, std::atoi (argv[1]));

		io_context.run ();
	}
	catch (std::exception & e) {
		std::cerr << "Exception: " << e.what () << std::endl;
	}

	return 0;
}
