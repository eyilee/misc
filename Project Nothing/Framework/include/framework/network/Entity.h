#pragma once

class CNetBridge;

class IEntity
{
public:
	IEntity ();
	virtual ~IEntity ();

	void set_net_bridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

protected:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};

template<typename T>
class CEntity : public IEntity
{
public:
	CEntity ();
	virtual ~CEntity ();
};

template<typename T>
inline CEntity<T>::CEntity ()
{
}

template<typename T>
inline CEntity<T>::~CEntity ()
{
}