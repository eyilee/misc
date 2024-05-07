#pragma once
#include "framework/manager/BaseManager.h"

class CNetBridge;
class CTcpListener;
class CUdpSession;

class CNetworkManager : public CBaseManager<CNetworkManager>
{
public:
	CNetworkManager (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort);
	virtual ~CNetworkManager ();

	static void Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, unsigned short _nPort);
	static void Shutdown ();

	static void TcpAccept (boost::asio::ip::tcp::socket& _rkSocket);
	static std::shared_ptr<CUdpSession> UdpConnect (std::shared_ptr<CNetBridge> _pkNetBridge, unsigned short _nPort);

	static std::shared_ptr<CNetBridge> GetNetBridge (uint32_t _nID);
	static void RemoveNetBridge (uint32_t _nID);

private:
	void Run ();
	void Stop ();

	std::shared_ptr<CNetBridge> CreateNetBridge ();

private:
	boost::asio::io_context& m_rkContext;
	std::string m_kHostAddr;
	unsigned short m_nPort;

	std::shared_ptr<CTcpListener> m_pkListener;

	std::map<uint32_t, std::shared_ptr<CNetBridge>> m_kNetBridges;
};

