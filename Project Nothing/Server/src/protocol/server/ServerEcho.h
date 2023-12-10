#pragma once

class ServerEcho : public CNetProtocol<ServerEcho>
{
public:
	ServerEcho ();
	virtual ~ServerEcho ();

	virtual void serialize (COutStream& _rkOutStream) override;
	virtual void deserialize (CInStream& _rkInStream) override;
	virtual void excute () override;

private:
	std::string m_kString;
};