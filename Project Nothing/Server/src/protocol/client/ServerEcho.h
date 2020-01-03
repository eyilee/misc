#pragma once

class INetProtocol;

class ServerEcho : public INetProtocol
{
public:
	ServerEcho ();
	virtual ~ServerEcho ();

	virtual void serialize (COutStream& _kOut_Stream) override;
	virtual void deserialize (CInStream& _kIn_Stream) override;
	virtual void excute () override;

private:
	int m_nNum;
};