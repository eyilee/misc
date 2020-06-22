#include "stdafx.h"

#include "CConfigLoader.h"

namespace po = boost::program_options;

CConfigLoader::CConfigLoader ()
{
}

CConfigLoader::~CConfigLoader ()
{
}

void CConfigLoader::load ()
{
	po::options_description kServer_desc ("config");
	kServer_desc.add_options ()
		("server.host", po::value<std::string> ())
		("server.port", po::value<std::string> ())
		;

	po::store (po::parse_config_file ("config.ini", kServer_desc), m_kVM);
	po::notify (m_kVM);

	if (m_kVM.count ("server.host")) {
		std::cout << "server.host: " << m_kVM["server.host"].as<std::string> () << std::endl;
	}

	if (m_kVM.count ("server.port")) {
		std::cout << "server.port: " << m_kVM["server.port"].as<std::string> () << std::endl;
	}
}
