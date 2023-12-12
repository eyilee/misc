#pragma once

class CNetBridge;

class IEntity
{
public:
	IEntity ();
	virtual ~IEntity ();

	int GetID () { return m_nID; }
	void SetID (const int& _rnID) { m_nID = _rnID; }

	std::shared_ptr<CNetBridge> GetNetBridge () { return m_pkNetBridge; }
	void SetNetBridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

protected:
	int m_nID;
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};
