#pragma once
#include "framework/manager/BaseManager.h"

class INetProtocol;
class INetProtocolGenerator;
template<typename T>
class CNetProtocolGenerator;

class CProtocolManager : public CBaseManager<CProtocolManager>
{
public:
	CProtocolManager ();
	virtual ~CProtocolManager ();

	void init ();
	void shutdown ();

	template<typename T>
	void register_protocol (unsigned short _nProtocolID);

	std::shared_ptr<INetProtocol> generate_protocol (unsigned short _nProtocolID);

private:
	std::map<int, std::shared_ptr<INetProtocolGenerator>> m_kProtocolMap;
};

template<typename T>
inline void CProtocolManager::register_protocol (unsigned short _nProtocolID)
{
	std::shared_ptr<INetProtocolGenerator> generator = std::make_shared<CNetProtocolGenerator<T>> ();

	if (m_kProtocolMap.find (_nProtocolID) == m_kProtocolMap.end ())
	{
		T::set_protocol_id (_nProtocolID);
		m_kProtocolMap.emplace (_nProtocolID, generator);
	}
}
