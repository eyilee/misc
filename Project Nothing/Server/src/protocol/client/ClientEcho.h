#pragma once

class ClientEcho : public CNetProtocol<ClientEcho>
{
public:
	ClientEcho ();
	ClientEcho (const std::string& _rkString);
	virtual ~ClientEcho ();

	virtual void serialize (COutStream& _rkOutStream) override;
	virtual void deserialize (CInStream& _rkInStream) override;
	virtual void excute () override;

private:
	std::string m_kString;
};