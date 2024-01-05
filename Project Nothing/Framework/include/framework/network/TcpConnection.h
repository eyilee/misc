#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::tcp;

class CNetBridge;
class CTcpSession;

class CTcpConnection : public std::enable_shared_from_this<CTcpConnection>
{
public:
	CTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, std::shared_ptr<CTcpSession> _pkTcpSession);
	virtual ~CTcpConnection ();

	void Init ();
	void Shutdown ();

	inline tcp::endpoint GetEndpoint () const { return m_kEndPoint; }

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
	std::shared_ptr<CTcpSession> m_pkTcpSession;
	tcp::endpoint m_kEndPoint;
};