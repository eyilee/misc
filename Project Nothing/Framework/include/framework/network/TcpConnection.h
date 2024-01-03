#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::tcp;

class CNetBridge;
class CTcpSession;

class CTcpConnection : public std::enable_shared_from_this<CTcpConnection>
{
public:
	CTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket);
	virtual ~CTcpConnection ();

	void Init ();
	void Shutdown ();

	inline uint32_t GetIP () const { return m_nIP; }

	void ResolveInput (CBitInStream& _rkInStream);
	void ComposeOutput (CBitOutStream& _rkOutStream);

private:
	std::shared_ptr<CNetBridge> m_pkNetBridge;
	std::shared_ptr<CTcpSession> m_pkTcpSession;
	uint32_t m_nIP;
};