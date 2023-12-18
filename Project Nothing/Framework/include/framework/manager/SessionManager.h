#pragma once
#include "framework/manager/BaseManager.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class CTcpListener;
class CTcpSession;
class CUdpSession;

class CSessionManager : public CBaseManager<CSessionManager>
{
public:
	CSessionManager ();
	virtual ~CSessionManager ();

	static void Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	static void Shutdown ();
	
	static void CreateTcpSession (tcp::socket& _rkSocket);

	static std::shared_ptr<CUdpSession> GetUdpSession ();

private:
	void Run (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nTcpPort, short _nUdpPort);
	void Stop ();

	void Create (tcp::socket& _rkSocket);
	void Queue (std::shared_ptr<CTcpSession> _pkTcpSession);

private:
	std::shared_ptr<CTcpListener> m_pkListener;
	std::list<std::shared_ptr<CTcpSession>> m_pkSessionList;
	std::shared_ptr<CUdpSession> m_pkUdpSession;
};

