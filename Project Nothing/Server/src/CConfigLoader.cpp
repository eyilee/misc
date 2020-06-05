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
	po::options_description kDB_desc ("config");
	kDB_desc.add_options ()
		("server.port", po::value<int> ())
		("db.user", po::value<std::string> ())
		("db.password", po::value<std::string> ())
		("db.dbname", po::value<std::string> ())
		("db.hostaddr", po::value<std::string> ())
		;

	po::store (po::parse_config_file ("config.ini", kDB_desc), m_kVM);
	po::notify (m_kVM);

	if (m_kVM.count ("server.port")) {
		std::cout << "server.port: " << m_kVM["server.port"].as<int> () << std::endl;
	}

	if (m_kVM.count ("db.user")) {
		std::cout << "db.user: " << m_kVM["db.user"].as<std::string> () << std::endl;
	}

	if (m_kVM.count ("db.password")) {
		std::cout << "db.password: " << m_kVM["db.password"].as<std::string> () << std::endl;
	}

	if (m_kVM.count ("db.dbname")) {
		std::cout << "db.dbname: " << m_kVM["db.dbname"].as<std::string> () << std::endl;
	}

	if (m_kVM.count ("db.hostaddr")) {
		std::cout << "db.hostaddr: " << m_kVM["db.hostaddr"].as<std::string> () << std::endl;
	}
}
