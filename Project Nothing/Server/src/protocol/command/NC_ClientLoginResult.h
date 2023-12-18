#pragma once

class NC_ClientLoginResult : public CNetCommand<NC_ClientLoginResult>
{
public:
	NC_ClientLoginResult ();
	NC_ClientLoginResult (int _nID, uint32_t _nKey);
	virtual ~NC_ClientLoginResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	int m_nID;
	uint32_t m_nKey;
};