#include "stdafx.h"
#include "ConfigLoader.h"

po::variables_map CConfigLoader::m_kVM;

CConfigLoader::CConfigLoader ()
{
}

CConfigLoader::~CConfigLoader ()
{
}

void CConfigLoader::Load ()
{
	po::options_description description ("config");
	description.add_options ()
		("server.hostaddr", po::value<std::string> ())
		("server.port", po::value<unsigned short> ())
		("game.tickrate", po::value<unsigned short> ())
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

	if (m_kVM.count ("server.port")) {
		LOG_INFO ("server.tcpport: %hu", m_kVM["server.port"].as<unsigned short> ());
	}

	if (m_kVM.count ("game.tickrate")) {
		LOG_INFO ("game.tickrate: %hu", m_kVM["game.tickrate"].as<unsigned short> ());
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
