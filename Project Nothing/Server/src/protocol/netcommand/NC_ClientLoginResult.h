#pragma once

class NC_ClientLoginResult : public INetCommand<NC_ClientLoginResult>
{
public:
	NC_ClientLoginResult ();
	NC_ClientLoginResult (uint32_t _nID);
	virtual ~NC_ClientLoginResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	uint32_t m_nID;
};
