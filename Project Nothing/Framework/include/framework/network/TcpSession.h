#pragma once

using boost::asio::ip::tcp;

class CNetBridge;
class COutStream;

constexpr size_t TCP_SESSION_BUFFER_SIZE = 8192;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
	friend CNetBridge;

public:
	CTcpSession (tcp::socket& _rkSocket);
	virtual ~CTcpSession ();

	void init ();
	void shutdown ();

private:
	void async_read ();
	void async_write (std::size_t _nBytes);

	void on_read (const boost::asio::const_buffer& _rkBuffer);
	void on_write (const COutStream& _rkOutStream);

private:
	tcp::socket m_kSocket;

	std::array<char, TCP_SESSION_BUFFER_SIZE> m_kWriteBuffer;
	std::array<char, TCP_SESSION_BUFFER_SIZE> m_kReadBuffer;

	std::shared_ptr<CNetBridge> m_pkNetBridge;
};