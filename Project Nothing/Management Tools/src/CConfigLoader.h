#pragma once

namespace po = boost::program_options;

class CConfigLoader
{
public:
	CConfigLoader ();
	virtual ~CConfigLoader ();

	void load ();

	template<typename T>
	T get_config (const std::string& _kKey);

private:
	po::variables_map m_kVM;
};

template<typename T>
inline T CConfigLoader::get_config (const std::string& _kKey)
{
	if (m_kVM.count (_kKey)) {
		return m_kVM[_kKey].as<T> ();
	}

	return T ();
}
