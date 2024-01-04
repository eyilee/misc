#pragma once
#include "framework/manager/BaseManager.h"

using boost::asio::ip::tcp;

class CNetBridge;
class CTcpListener;
class CUdpSession;

class CNetworkManager : public CBaseManager<CNetworkManager>
{
public:
	CNetworkManager (boost::asio::io_context& _rkContext);
	virtual ~CNetworkManager ();

	static void Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	static void Shutdown ();

	static void TcpAccept (tcp::socket& _rkSocket);
	static void UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge, short _nUdpPort);

	static std::shared_ptr<CNetBridge> GetNetBridge (uint32_t _nID);

private:
	void Run (const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	void Stop ();

	std::shared_ptr<CNetBridge> CreateNetBridge ();

	void SetupTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket);
	void SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, short _nUdpPort);

private:
	boost::asio::io_context& m_rkContext;
	std::string m_kHostAddr;
	short m_nTcpPort;
	short m_nUdpPort;

	std::shared_ptr<CTcpListener> m_pkListener;

	std::map<uint32_t, std::shared_ptr<CNetBridge>> m_pkNetBridges;
};

