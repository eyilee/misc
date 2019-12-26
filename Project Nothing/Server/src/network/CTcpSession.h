#pragma once

using asio::ip::tcp;

class CNetBridge;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
public:
	CTcpSession (tcp::socket& _socket);
	virtual ~CTcpSession ();

	void init ();

private:
	void async_read ();
	void async_write (std::size_t _nLength);

	void on_read (const asio::const_buffer& _kBuffer);
	void on_write ();

private:
	tcp::socket m_kSocket;

	std::array<char, 1024> m_kSend_buffer;
	std::array<char, 1024> m_kReceive_buffer;

	std::shared_ptr<CNetBridge> m_pNet_bridge;
};