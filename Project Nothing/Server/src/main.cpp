#include "stdafx.h"

int main (int argc, char* argv[])
{
	std::shared_ptr<CServer> pServer = std::make_shared<CServer> ();

	pServer->init ();
	pServer->run ();

	return 0;
}
