#pragma once
#include "framework/manager/BaseManager.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class CNetBridge;
class CTcpListener;

class CNetworkManager : public CBaseManager<CNetworkManager>
{
public:
	CNetworkManager (boost::asio::io_context& _rkContext);
	virtual ~CNetworkManager ();

	static void Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort);
	static void Shutdown ();

	static void TcpAccept (tcp::socket& _rkSocket);
	static void UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge, unsigned short _nPort);

	static std::shared_ptr<CNetBridge> GetNetBridge (uint32_t _nID);
	static void RemoveNetBridge (uint32_t _nID);

private:
	void Run (const std::string& _rkHostAddr, unsigned short _nPort);
	void Stop ();

	std::shared_ptr<CNetBridge> CreateNetBridge ();

	void SetupTcpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, tcp::socket& _rkSocket);
	void SetupUdpConnection (std::shared_ptr<CNetBridge> _pkNetBridge, udp::socket& _rkSocket);

private:
	boost::asio::io_context& m_rkContext;
	std::string m_kHostAddr;
	unsigned short m_nPort;

	std::shared_ptr<CTcpListener> m_pkListener;

	std::map<uint32_t, std::shared_ptr<CNetBridge>> m_pkNetBridges;
};

