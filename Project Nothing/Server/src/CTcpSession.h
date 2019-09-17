#pragma once

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
public:
	CTcpSession (const std::shared_ptr<CTcpConnection>& _kConnection);
	virtual ~CTcpSession ();

private:
	std::shared_ptr<CTcpConnection> m_kConnection;
};