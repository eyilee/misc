#pragma once

class ServerUdpConnect : public CNetProtocol<ServerUdpConnect>
{
public:
	ServerUdpConnect ();
	virtual ~ServerUdpConnect ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;
	virtual void Excute () override;
};