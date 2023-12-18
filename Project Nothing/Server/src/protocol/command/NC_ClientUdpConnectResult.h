#pragma once

class NC_ClientUdpConnectResult : public CNetProtocol<NC_ClientUdpConnectResult>
{
public:
	NC_ClientUdpConnectResult ();
	virtual ~NC_ClientUdpConnectResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	uint32_t m_nKey;
};