#pragma once

class CServerConnection : public CUdpConnection
{
public:
	CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession);
	virtual ~CServerConnection ();

protected:
	virtual void ResolvePackage (CBitInStream& _rkInStream) override;
	virtual void OnPacketAcked (uint32_t _nSequence, SOutPacket& _rkOutPacket) override;
};