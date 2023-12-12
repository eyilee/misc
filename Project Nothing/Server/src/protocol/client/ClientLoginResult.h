#pragma once

class ClientLoginResult : public CNetProtocol<ClientLoginResult>
{
public:
	ClientLoginResult ();
	virtual ~ClientLoginResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};