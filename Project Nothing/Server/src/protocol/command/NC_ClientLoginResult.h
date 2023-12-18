#pragma once

class NC_ClientLoginResult : public CNetProtocol<NC_ClientLoginResult>
{
public:
	NC_ClientLoginResult ();
	NC_ClientLoginResult (int _nID, uint32_t _nKey);
	virtual ~NC_ClientLoginResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	int m_nID;
	uint32_t m_nKey;
};