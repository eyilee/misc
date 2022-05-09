#pragma once

class ClientEcho : public CNetProtocol<ClientEcho>
{
public:
	ClientEcho ();
	ClientEcho (const std::string& _kString);
	virtual ~ClientEcho ();

	virtual void serialize (COutStream& _kOut_stream) override;
	virtual void deserialize (CInStream& _kIn_stream) override;
	virtual void excute () override;

private:
	std::string m_kString;
};