#pragma once
#include "framework/manager/BaseManager.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class CNetBridge;
class CTcpListener;
class CUdpSession;

class CNetworkManager : public CBaseManager<CNetworkManager>
{
public:
	CNetworkManager ();
	virtual ~CNetworkManager ();

	static void Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	static void Shutdown ();

	static void TcpAccept (tcp::socket& _rkSocket);
	static void UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge);

private:
	void Run (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	void Stop ();

	void SetupTcpConnection (tcp::socket& _rkSocket);
	void SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge);

private:
	std::shared_ptr<CTcpListener> m_pkListener;
	std::shared_ptr<CUdpSession> m_pkUdpSession;

	uint32_t m_nConnectionID;
	std::map<uint32_t, std::shared_ptr<CNetBridge>> m_pkNetBridges;
};

