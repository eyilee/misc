#pragma once

namespace po = boost::program_options;

class CConfigLoader
{
public:
	CConfigLoader ();
	virtual ~CConfigLoader ();

	void Load ();

	template<typename T>
	T GetConfig (const std::string& _rkKey);

private:
	po::variables_map m_kVM;
};

template<typename T>
inline T CConfigLoader::GetConfig (const std::string& _rkKey)
{
	if (m_kVM.count (_rkKey)) {
		return m_kVM[_rkKey].as<T> ();
	}

	return T ();
}
