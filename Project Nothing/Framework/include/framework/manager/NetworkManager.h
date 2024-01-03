#pragma once
#include "framework/manager/BaseManager.h"

using boost::asio::ip::tcp;

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

	static std::shared_ptr<CNetBridge> GetNetBridge (uint32_t _nID);

private:
	void Run (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	void Stop ();

	std::shared_ptr<CNetBridge> CreateNetBridge ();

	void SetupTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket);
	void SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge);

private:
	std::shared_ptr<CTcpListener> m_pkListener;
	std::shared_ptr<CUdpSession> m_pkUdpSession;

	std::map<uint32_t, std::shared_ptr<CNetBridge>> m_pkNetBridges;
};

