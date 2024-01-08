#pragma once
#include "framework/network/BitStream.h"

class CNetBridge;

class INetProtocol
{
public:
	INetProtocol ();
	virtual ~INetProtocol ();

	inline void SetNetBridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

	virtual void OnSerialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Serialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Deserialize (CBitInStream& _rkInStream) = 0;
	virtual void Excute () = 0;

protected:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};

template<typename T>
class INetCommand : public INetProtocol
{
public:
	INetCommand ();
	virtual ~INetCommand ();

	static unsigned short GetID () { return m_nID; };
	static void SetID (unsigned short _nID) { m_nID = _nID; }

	virtual void OnSerialize (CBitOutStream& _rkOutStream) final;
	virtual void Serialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Deserialize (CBitInStream& _rkInStream) final {}
	virtual void Excute () final {}

private:
	static unsigned short m_nID;
};

template<typename T>
inline INetCommand<T>::INetCommand ()
{
}

template<typename T>
inline INetCommand<T>::~INetCommand ()
{
}

template<typename T>
inline void INetCommand<T>::OnSerialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nID);
	Serialize (_rkOutStream);
}

template<typename T>
unsigned short INetCommand<T>::m_nID;

template<typename T>
class INetEvent : public INetProtocol
{
public:
	INetEvent ();
	virtual ~INetEvent ();

	static unsigned short GetID () { return m_nID; };
	static void SetID (unsigned short _nID) { m_nID = _nID; }

	virtual void OnSerialize (CBitOutStream& _rkOutStream) final {}
	virtual void Serialize (CBitOutStream& _rkOutStream) final {}
	virtual void Deserialize (CBitInStream& _rkInStream) = 0;
	virtual void Excute () = 0;

private:
	static unsigned short m_nID;
};

template<typename T>
inline INetEvent<T>::INetEvent ()
{
}

template<typename T>
inline INetEvent<T>::~INetEvent ()
{
}

template<typename T>
unsigned short INetEvent<T>::m_nID;
