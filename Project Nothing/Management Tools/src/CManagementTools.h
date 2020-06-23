#pragma once

class CConfigLoader;

class CTcpClient;

class CManagementTools : public std::enable_shared_from_this<CManagementTools>
{
public:
	CManagementTools ();
	virtual ~CManagementTools ();

	void init ();

	void init_protocol_manager ();
	void init_tcp_client ();

	void run ();

public:
	static std::shared_ptr<CManagementTools> Instance;

private:

	boost::asio::io_context m_kIo_context;

	CConfigLoader m_kConfig_loader;

	std::shared_ptr<CTcpClient> m_pTcp_client;
};