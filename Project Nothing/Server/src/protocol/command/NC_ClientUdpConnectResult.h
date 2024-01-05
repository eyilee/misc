#pragma once

class NC_ClientUdpConnectResult : public CNetCommand<NC_ClientUdpConnectResult>
{
public:
	NC_ClientUdpConnectResult ();
	NC_ClientUdpConnectResult (bool _bResult, unsigned short _nPort);
	virtual ~NC_ClientUdpConnectResult ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;

private:
	bool m_bResult;
	unsigned short m_nPort;
};