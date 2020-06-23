#pragma once

#include "network/CNetProtocolGenerator.h"

#include "manager/CBaseManager.h"

class CNetBridge;
class INetProtocol;

class CProtocolManager : public CBaseManager<CProtocolManager>
{
public:
	CProtocolManager ();
	virtual ~CProtocolManager ();

	void init ();

	std::shared_ptr<INetProtocol> generate_protocol (const unsigned short _nProtocol_id, std::shared_ptr<CNetBridge>& _pNet_Bridge);

private:
	template <typename T>
	void register_protocol (const unsigned short _nProtocol_id);

private:
	std::map<int, std::shared_ptr<INetProtocolGenerator>> m_kProtocol_map;
};

template <typename T>
inline void CProtocolManager::register_protocol (const unsigned short _nProtocol_id)
{
	std::shared_ptr<INetProtocolGenerator> pProtocol_generator = std::make_shared<CNetProtocolGenerator<T>> ();

	if (m_kProtocol_map.find (_nProtocol_id) == m_kProtocol_map.end ())
	{
		T::set_protocol_id (_nProtocol_id);
		m_kProtocol_map.emplace (_nProtocol_id, pProtocol_generator);
	}
}
