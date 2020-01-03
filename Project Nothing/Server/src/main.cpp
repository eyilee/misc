#include "stdafx.h"

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: server <port>" << std::endl;
		return 1;
	}

	std::shared_ptr<CServer> pServer = std::make_shared<CServer> ();

	pServer->init (std::atoi (argv[1]));
	pServer->run ();

	return 0;
}
