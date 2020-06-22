#pragma once

class CProtocolManager;
class CTcpClient;

class CManagementTools : public std::enable_shared_from_this<CManagementTools>
{
public:
	CManagementTools ();
	virtual ~CManagementTools ();

	void init ();
	void run ();

private:
	static std::shared_ptr<CManagementTools> m_Instance;

	boost::asio::io_context m_kIo_context;

	std::shared_ptr<CProtocolManager> m_pProtocol_manager;
	std::shared_ptr<CTcpClient> m_pTcp_client;
};