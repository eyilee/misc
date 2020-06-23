#include "stdafx.h"

#include "CConfigLoader.h"
#include "CManagementTools.h"

int main (int argc, char* argv[])
{
	std::shared_ptr<CManagementTools> pManagementTools = std::make_shared<CManagementTools> ();

	pManagementTools->init ();
	pManagementTools->run ();

	return 0;
}
