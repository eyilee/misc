#include "stdafx.h"

#include "CInStream.h"
#include "COutStream.h"

#include "CTcpConnection.h"
#include "CTcpSession.h"
#include "CTcpListener.h"

static std::map<int, std::shared_ptr<CTcpSession>> g_kSession_manager;

int main (int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
		}

		asio::io_context io_context;

		CTcpListener kListener (io_context, std::atoi (argv[1]));

		kListener.Init (g_kSession_manager);

		io_context.run ();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what () << "\n";
	}

	return 0;
}
