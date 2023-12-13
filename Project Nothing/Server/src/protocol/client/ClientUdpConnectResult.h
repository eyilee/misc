#pragma once

class ClientUdpConnectResult : public CNetProtocol<ClientUdpConnectResult>
{
public:
	ClientUdpConnectResult ();
	virtual ~ClientUdpConnectResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;

private:
	uint32_t m_nKey;
};