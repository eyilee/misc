#pragma once

class CNetBridge;

class IEntity
{
public:
	IEntity ();
	virtual ~IEntity ();

	inline int GetID () const { return m_nID; }
	inline void SetID (int _nID) { m_nID = _nID; }

	std::shared_ptr<CNetBridge> GetNetBridge () { return m_pkNetBridge; }
	void SetNetBridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

protected:
	int m_nID;
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};
