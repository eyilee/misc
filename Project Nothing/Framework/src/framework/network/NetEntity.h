#pragma once

class CNetBridge;

class INetEntity
{
public:
	INetEntity ();
	virtual ~INetEntity ();

	inline uint32_t GetID () const { return m_nID; }
	inline void SetID (uint32_t _nID) { m_nID = _nID; }

	std::shared_ptr<CNetBridge> GetNetBridge () { return m_pkNetBridge; }
	void SetNetBridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

	virtual void OnConnected () {}
	virtual void OnDisconnected () {}

protected:
	uint32_t m_nID;
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};
