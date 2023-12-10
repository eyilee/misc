#pragma once

class CInStream;
class CNetBridge;
class COutStream;

class INetProtocol
{
public:
	INetProtocol ();
	virtual ~INetProtocol ();

	void set_net_bridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; };

	virtual void on_serialize (COutStream& _rkOutStream) = 0;
	virtual void serialize (COutStream& _rkOutStream) = 0;
	virtual void deserialize (CInStream& _rkInStream) = 0;
	virtual void excute () = 0;

protected:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
};

template<typename T>
class CNetProtocol : public INetProtocol
{
public:
	CNetProtocol ();
	virtual ~CNetProtocol ();

	static const unsigned short get_protocol_id () { return m_nProtocolID; };
	static void set_protocol_id (const unsigned short _nProtocolID) { m_nProtocolID = _nProtocolID; }

	virtual void on_serialize (COutStream& _rkOutStream);
	virtual void serialize (COutStream& _rkOutStream) = 0;
	virtual void deserialize (CInStream& _rkInStream) = 0;
	virtual void excute () = 0;

private:
	static unsigned short m_nProtocolID;
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
inline void CNetProtocol<T>::on_serialize (COutStream& _rkOutStream)
{
	_rkOutStream << m_nProtocolID;
	serialize (_rkOutStream);
}

template<typename T>
unsigned short CNetProtocol<T>::m_nProtocolID;
