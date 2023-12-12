#pragma once

class ClientLoginResult : public CNetProtocol<ClientLoginResult>
{
public:
	ClientLoginResult ();
	virtual ~ClientLoginResult ();

	virtual void serialize (CBitOutStream& _rkOutStream) override;
	virtual void deserialize (CBitInStream& _rkInStream) override;
	virtual void excute () override;
};