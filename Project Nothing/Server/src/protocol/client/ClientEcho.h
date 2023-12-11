#pragma once

class ClientEcho : public CNetProtocol<ClientEcho>
{
public:
	ClientEcho ();
	ClientEcho (const std::wstring& _rkString);
	virtual ~ClientEcho ();

	virtual void serialize (CBitOutStream& _rkOutStream) override;
	virtual void deserialize (CBitInStream& _rkInStream) override;
	virtual void excute () override;

private:
	std::wstring m_kString;
};