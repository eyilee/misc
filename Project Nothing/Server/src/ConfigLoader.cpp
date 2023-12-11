#include "stdafx.h"
#include "ConfigLoader.h"

CConfigLoader::CConfigLoader ()
{
}

CConfigLoader::~CConfigLoader ()
{
}

void CConfigLoader::load ()
{
	po::options_description description ("config");
	description.add_options ()
		("server.hostaddr", po::value<std::string> ())
		("server.tcpport", po::value<short> ())
		("server.udpport", po::value<short> ())
		("db.user", po::value<std::string> ())
		("db.password", po::value<std::string> ())
		("db.dbname", po::value<std::string> ())
		("db.hostaddr", po::value<std::string> ())
		;

	po::store (po::parse_config_file ("config.ini", description), m_kVM);
	po::notify (m_kVM);

	if (m_kVM.count ("server.hostaddr")) {
		LOG_INFO ("server.hostaddr: %s", m_kVM["server.hostaddr"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("server.tcpport")) {
		LOG_INFO ("server.tcpport: %hd", m_kVM["server.tcpport"].as<short> ());
	}

	if (m_kVM.count ("server.udpport")) {
		LOG_INFO ("server.udpport: %hd", m_kVM["server.udpport"].as<short> ());
	}

	if (m_kVM.count ("db.user")) {
		LOG_INFO ("db.user: %s", m_kVM["db.user"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("db.password")) {
		LOG_INFO ("db.password: %s", m_kVM["db.password"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("db.dbname")) {
		LOG_INFO ("db.dbname: %s", m_kVM["db.dbname"].as<std::string> ().c_str ());
	}

	if (m_kVM.count ("db.hostaddr")) {
		LOG_INFO ("db.hostaddr: %s", m_kVM["db.hostaddr"].as<std::string> ().c_str ());
	}
}
