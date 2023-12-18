#pragma once

class NC_ClientEchoResult : public CNetProtocol<NC_ClientEchoResult>
{
public:
	NC_ClientEchoResult ();
	NC_ClientEchoResult (const std::wstring& _rkString);
	virtual ~NC_ClientEchoResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	std::wstring m_kString;
};