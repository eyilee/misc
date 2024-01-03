#pragma once

class NC_ClientLoginResult : public CNetCommand<NC_ClientLoginResult>
{
public:
	NC_ClientLoginResult ();
	NC_ClientLoginResult (uint32_t _nID, uint32_t _nKey);
	virtual ~NC_ClientLoginResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	uint32_t m_nID;
	uint32_t m_nKey;
};