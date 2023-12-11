#pragma once

class ServerEcho : public CNetProtocol<ServerEcho>
{
public:
	ServerEcho ();
	virtual ~ServerEcho ();

	virtual void serialize (CBitOutStream& _rkOutStream) override;
	virtual void deserialize (CBitInStream& _rkInStream) override;
	virtual void excute () override;

private:
	std::wstring m_kString;
};