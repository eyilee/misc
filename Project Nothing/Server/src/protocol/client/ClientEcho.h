#pragma once

class ClientEcho : public CNetProtocol<ClientEcho>
{
public:
	ClientEcho ();
	ClientEcho (const std::wstring& _rkString);
	virtual ~ClientEcho ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	std::wstring m_kString;
};