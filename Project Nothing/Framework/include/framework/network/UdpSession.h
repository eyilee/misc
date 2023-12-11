#pragma once

class CBitOutStream;

using boost::asio::ip::udp;

constexpr size_t UDP_SESSION_BUFFER_SIZE = 8192;

class CUdpSession : public std::enable_shared_from_this<CUdpSession>
{
public:
	CUdpSession (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort);
	virtual ~CUdpSession ();

	void init ();
	void shutdown ();

private:
	void async_receive ();
	void async_send (std::size_t _nBytes);

	void on_receive (const boost::asio::const_buffer& _rkBuffer);
	void on_send (const CBitOutStream& _rkOutStream);

private:
	udp::socket m_kSocket;
	udp::endpoint m_kEndpoint;

	std::array<uint8_t, UDP_SESSION_BUFFER_SIZE> m_kSendBuffer;
	std::array<uint8_t, UDP_SESSION_BUFFER_SIZE> m_kReceiveBuffer;
};