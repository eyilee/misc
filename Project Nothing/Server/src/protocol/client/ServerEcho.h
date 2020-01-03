#pragma once

template <typename T>
class CNetProtocol;

class ServerEcho : public CNetProtocol<ServerEcho>
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