#pragma once

class CBitOutStream;
class CNetBridge;

using boost::asio::ip::udp;

constexpr size_t UDP_SESSION_BUFFER_SIZE = 8192;

struct SUdpSendCommand
{
	std::vector<uint8_t> m_kBytes;
	udp::endpoint m_kEndPoint;

	SUdpSendCommand (const std::vector<uint8_t>& _rkBytes, const udp::endpoint& _rkEndPoint)
		: m_kBytes (_rkBytes)
		, m_kEndPoint (_rkEndPoint)
	{
	}
};

class CUdpSession : public std::enable_shared_from_this<CUdpSession>
{
	friend CNetBridge;

public:
	CUdpSession (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort);
	virtual ~CUdpSession ();

	void Init ();
	void Shutdown ();

private:
	void AsyncReceive ();
	void AsyncSend ();

	void OnReceive (const boost::asio::const_buffer& _rkBuffer);
	void OnSend (const CBitOutStream& _rkOutStream, const udp::endpoint& _rkEndPoint);

private:
	udp::socket m_kSocket;
	udp::endpoint m_kEndpoint;

	std::array<uint8_t, UDP_SESSION_BUFFER_SIZE> m_kSendBuffer;
	std::deque<SUdpSendCommand> m_kSendQueue;
	std::array<uint8_t, UDP_SESSION_BUFFER_SIZE> m_kReceiveBuffer;
};