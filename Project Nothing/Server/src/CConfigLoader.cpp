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
		("server.port", po::value<std::string> ())
		("db.user", po::value<std::string> ())
		("db.password", po::value<std::string> ())
		("db.dbname", po::value<std::string> ())
		("db.hostaddr", po::value<std::string> ())
		;

	po::variables_map vm;
	po::store (po::parse_config_file ("config.ini.example", kDB_desc), vm);
	po::notify (vm);

	if (vm.count ("server.port")) {
		std::cout << vm["server.port"].as<std::string> () << std::endl;
	}

	if (vm.count ("db.user")) {
		std::cout << vm["db.user"].as<std::string> () << std::endl;
	}

	if (vm.count ("db.password")) {
		std::cout << vm["db.password"].as<std::string> () << std::endl;
	}

	if (vm.count ("db.dbname")) {
		std::cout << vm["db.dbname"].as<std::string> () << std::endl;
	}

	if (vm.count ("db.hostaddr")) {
		std::cout << vm["db.hostaddr"].as<std::string> () << std::endl;
	}
}
