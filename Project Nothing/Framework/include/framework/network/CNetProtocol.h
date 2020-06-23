#pragma once

class CInStream;
class CNetBridge;
class COutStream;

class INetProtocol
{
public:
	INetProtocol ();
	virtual ~INetProtocol ();

	void set_net_bridge (std::shared_ptr<CNetBridge>& _pNet_Bridge) { m_pNet_Bridge = _pNet_Bridge; };

	virtual void on_serialize (COutStream& _kOut_Stream) = 0;
	virtual void serialize (COutStream& _kOut_Stream) = 0;
	virtual void deserialize (CInStream& _kIn_Stream) = 0;
	virtual void excute () = 0;

protected:
	std::shared_ptr<CNetBridge> m_pNet_Bridge;
};

template <typename T>
class CNetProtocol : public INetProtocol
{
public:
	CNetProtocol ();
	virtual ~CNetProtocol ();

	static const unsigned short get_protocol_id () { return m_nProtocol_id; };
	static void set_protocol_id (const unsigned short _nProtocol_id) { m_nProtocol_id = _nProtocol_id; }

	virtual void on_serialize (COutStream& _kOut_Stream);
	virtual void serialize (COutStream& _kOut_Stream) = 0;
	virtual void deserialize (CInStream& _kIn_Stream) = 0;
	virtual void excute () = 0;

private:
	static unsigned short m_nProtocol_id;
};

template <typename T>
inline CNetProtocol<T>::CNetProtocol ()
{
}

template <typename T>
inline CNetProtocol<T>::~CNetProtocol ()
{
}

template <typename T>
inline void CNetProtocol<T>::on_serialize (COutStream& _kOut_Stream)
{
	_kOut_Stream << m_nProtocol_id;
	serialize (_kOut_Stream);
}

template <typename T>
unsigned short CNetProtocol<T>::m_nProtocol_id;
