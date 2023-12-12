#pragma once

using boost::asio::ip::tcp;

class CBitOutStream;
class CNetBridge;

constexpr size_t TCP_SESSION_BUFFER_SIZE = 8192;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
	friend CNetBridge;

public:
	CTcpSession (tcp::socket& _rkSocket);
	virtual ~CTcpSession ();

	void Init ();
	void Shutdown ();

private:
	void AsyncRead ();
	void AsyncWrite (std::size_t _nBytes);

	void OnRead (const boost::asio::const_buffer& _rkBuffer);
	void OnWrite (const CBitOutStream& _rkOutStream);

private:
	tcp::socket m_kSocket;

	std::array<uint8_t, TCP_SESSION_BUFFER_SIZE> m_kWriteBuffer;
	std::array<uint8_t, TCP_SESSION_BUFFER_SIZE> m_kReadBuffer;

	std::shared_ptr<CNetBridge> m_pkNetBridge;
};