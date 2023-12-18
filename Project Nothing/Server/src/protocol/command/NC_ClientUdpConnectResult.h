#pragma once

class NC_ClientUdpConnectResult : public CNetCommand<NC_ClientUdpConnectResult>
{
public:
	NC_ClientUdpConnectResult ();
	virtual ~NC_ClientUdpConnectResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	uint32_t m_nKey;
};