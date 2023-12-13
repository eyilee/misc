#pragma once

class ClientLoginResult : public CNetProtocol<ClientLoginResult>
{
public:
	ClientLoginResult ();
	ClientLoginResult (const uint32_t& _rnKey);
	virtual ~ClientLoginResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	uint32_t m_nKey;
};