#pragma once

class ServerEcho : public CNetProtocol<ServerEcho>
{
public:
	ServerEcho ();
	virtual ~ServerEcho ();

	virtual void serialize (COutStream& _kOut_stream) override;
	virtual void deserialize (CInStream& _kIn_stream) override;
	virtual void excute () override;

private:
	std::string m_kString;
};