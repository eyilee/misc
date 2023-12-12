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
class CNetProtocol : public INetProtocol
{
public:
	CNetProtocol ();
	virtual ~CNetProtocol ();

	static unsigned short GetID () { return m_nID; };
	static void SetID (const unsigned short& _rnID) { m_nID = _rnID; }

	virtual void OnSerialize (CBitOutStream& _rkOutStream);
	virtual void Serialize (CBitOutStream& _rkOutStream) = 0;
	virtual void Deserialize (CBitInStream& _rkInStream) = 0;
	virtual void Excute () = 0;

private:
	static unsigned short m_nID;
};

template<typename T>
inline CNetProtocol<T>::CNetProtocol ()
{
}

template<typename T>
inline CNetProtocol<T>::~CNetProtocol ()
{
}

template<typename T>
inline void CNetProtocol<T>::OnSerialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nID);
	Serialize (_rkOutStream);
}

template<typename T>
unsigned short CNetProtocol<T>::m_nID;
