#pragma once
#include "framework/manager/BaseManager.h"

class CTcpListener;
class CTcpSession;
class CUdpSession;

class CSessionManager : public CBaseManager<CSessionManager>
{
public:
	CSessionManager ();
	virtual ~CSessionManager ();

	static void Init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, const short _nTcpPort, const short _nUdpPort);
	static void Shutdown ();

	static void PushSession (std::shared_ptr<CTcpSession> _pkTcpSession);

private:
	void Run (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, const short _nTcpPort, const short _nUdpPort);
	void Stop ();

	void Queue (std::shared_ptr<CTcpSession> _pkTcpSession);

private:
	std::shared_ptr<CTcpListener> m_pkListener;
	std::list<std::shared_ptr<CTcpSession>> m_pkSessionList;
	std::shared_ptr<CUdpSession> m_pkUdpSession;
};

