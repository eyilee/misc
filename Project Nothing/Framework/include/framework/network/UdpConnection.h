#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::udp;

class CNetBridge;
class CUdpSession;

class CUdpConnection : public std::enable_shared_from_this<CUdpConnection>
{
public:
	CUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CUdpSession> _pkUdpSession);
	virtual ~CUdpConnection ();

	void Init ();
	void Shutdown ();

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (CBitOutStream& _rkOutStream);

private:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
	std::shared_ptr<CUdpSession> m_pkUdpSession;
	udp::endpoint m_kEndPoint;
};