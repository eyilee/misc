#include "stdafx.h"

#include "CServer.h"

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: server <port>" << std::endl;
		return 1;
	}

	CServer kServer;

	kServer.init (std::atoi (argv[1]));
	kServer.run ();

	return 0;
}
