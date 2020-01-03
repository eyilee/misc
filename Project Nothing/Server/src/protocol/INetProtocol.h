#pragma once

class CInStream;
class COutStream;
class CNetBridge;

class INetProtocol
{
public:
	INetProtocol ();
	virtual ~INetProtocol ();

	static const unsigned short get_protocol_id () { return m_nProtocol_id; };
	static void set_protocol_id (const unsigned short _nProtocol_id) { m_nProtocol_id = _nProtocol_id; }

	void set_net_bridge (std::shared_ptr<CNetBridge>& _pNet_Bridge) { m_pNet_Bridge = _pNet_Bridge; };

	virtual void serialize (COutStream& _kOut_Stream) = 0;
	virtual void deserialize (CInStream& _kIn_Stream) = 0;
	virtual void excute () = 0;

protected:
	std::shared_ptr<CNetBridge> m_pNet_Bridge;

private:
	static unsigned short m_nProtocol_id;
};