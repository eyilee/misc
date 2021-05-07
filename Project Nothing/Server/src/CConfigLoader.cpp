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
		LOG_EVENT ("server.port: %d", m_kVM["server.port"].as<int> ());
	}

	if (m_kVM.count ("db.user")) {
		LOG_EVENT ("db.user: %s", m_kVM["db.user"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("db.password")) {
		LOG_EVENT ("db.password: %s", m_kVM["db.password"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("db.dbname")) {
		LOG_EVENT ("db.dbname: %s", m_kVM["db.dbname"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("db.hostaddr")) {
		LOG_EVENT ("db.hostaddr: %s", m_kVM["db.hostaddr"].as<std::string> ().c_str ());
	}
}
