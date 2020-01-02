#pragma once

class CInStream;
class COutStream;

class INetProtocol
{
public:
	INetProtocol ();
	virtual ~INetProtocol ();

	const unsigned short get_protocol_id () const { return m_nProtocol_id; };

	virtual void serialize (COutStream& _kOut_Stream) {};
	virtual void deserialize (CInStream& _kIn_Stream) {};

	virtual void excute () {};

private:
	unsigned short m_nProtocol_id;
};