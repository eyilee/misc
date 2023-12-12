#pragma once

class ServerEcho : public CNetProtocol<ServerEcho>
{
public:
	ServerEcho ();
	virtual ~ServerEcho ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	std::wstring m_kString;
};