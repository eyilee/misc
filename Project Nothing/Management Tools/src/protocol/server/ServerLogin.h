#pragma once

class ServerLogin : public CNetProtocol<ServerLogin>
{
public:
	ServerLogin ();
	explicit ServerLogin (const int _nId);
	virtual ~ServerLogin ();

	virtual void serialize (COutStream& _kOut_Stream) override;
	virtual void deserialize (CInStream& _kIn_Stream) override;
	virtual void excute () override;

private:
	int m_nId;
};