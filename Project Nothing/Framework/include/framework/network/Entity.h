#pragma once

class CNetBridge;

class IEntity
{
public:
	IEntity ();
	virtual ~IEntity ();

	int get_id () { return m_nID; }
	void set_id (const int& _nID) { m_nID = _nID; }

	std::shared_ptr<CNetBridge> get_net_bridge () { return m_pkNetBridge; }
	void set_net_bridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

protected:
	int m_nID;
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};
