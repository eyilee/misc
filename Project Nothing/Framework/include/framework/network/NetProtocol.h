#pragma once
#include "framework/network/BitStream.h"

class CNetBridge;

class INetProtocol
{
public:
	INetProtocol ();
	virtual ~INetProtocol ();

	void SetNetBridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

	virtual void OnSerialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Serialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Deserialize (CBitInStream& _rkInStream) = 0;
	virtual void Excute () = 0;

protected:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};

template<typename T>
class CNetCommand : public INetProtocol
{
public:
	CNetCommand ();
	virtual ~CNetCommand ();

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
inline CNetCommand<T>::CNetCommand ()
{
}

template<typename T>
inline CNetCommand<T>::~CNetCommand ()
{
}

template<typename T>
inline void CNetCommand<T>::OnSerialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nID);
	Serialize (_rkOutStream);
}

template<typename T>
unsigned short CNetCommand<T>::m_nID;

template<typename T>
class CNetEvent : public INetProtocol
{
public:
	CNetEvent ();
	virtual ~CNetEvent ();

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
inline CNetEvent<T>::CNetEvent ()
{
}

template<typename T>
inline CNetEvent<T>::~CNetEvent ()
{
}

template<typename T>
unsigned short CNetEvent<T>::m_nID;
