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

	void init (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, const short _nTcpPort, const short _nUdpPort);
	void shutdown ();

	void add_session (std::shared_ptr<CTcpSession> _pkSession);

private:
	std::shared_ptr<CTcpListener> m_pkListener;

	std::list<std::shared_ptr<CTcpSession>> m_pkSessionList;

	std::shared_ptr<CUdpSession> m_pkUdpSession;
};

