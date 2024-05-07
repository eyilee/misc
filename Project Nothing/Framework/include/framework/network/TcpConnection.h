#pragma once
#include "framework/network/BitStream.h"

class INetProtocol;
class CNetBridge;
class CTcpSession;

class CTcpConnection : public std::enable_shared_from_this<CTcpConnection>
{
public:
	CTcpConnection (std::shared_ptr<CTcpSession> _pkTcpSession);
	virtual ~CTcpConnection ();

	inline std::shared_ptr<CNetBridge> GetNetBridge () const { return m_pkNetBridge; }
	inline void SetNetBridge (std::shared_ptr<CNetBridge> _pkNetBridge) { m_pkNetBridge = _pkNetBridge; }

	inline boost::asio::ip::tcp::endpoint GetRemoteEndpoint () const { return m_kRemoteEndPoint; }

	void Init ();
	void Shutdown ();
	void OnDisconnect ();

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (std::shared_ptr<INetProtocol> _pkProtocol);

private:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
	std::shared_ptr<CTcpSession> m_pkTcpSession;
	boost::asio::ip::tcp::endpoint m_kRemoteEndPoint;
};