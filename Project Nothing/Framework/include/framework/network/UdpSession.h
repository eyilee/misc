#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::udp;

class CUdpConnection;

constexpr size_t UDP_SOCKET_BUFFER_SIZE = 512;

class CUdpSession : public std::enable_shared_from_this<CUdpSession>
{
	friend CUdpConnection;

private:
	struct SSendCommand
	{
		std::vector<uint8_t> m_kBytes;
		udp::endpoint m_kEndPoint;

		SSendCommand (const std::vector<uint8_t>& _rkBytes, const udp::endpoint& _rkEndPoint)
			: m_kBytes (_rkBytes)
			, m_kEndPoint (_rkEndPoint)
		{
		}
	};

public:
	CUdpSession (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort);
	virtual ~CUdpSession ();

	void Init ();
	void Shutdown ();

private:
	void AsyncReceive ();
	void OnReceive (size_t _nLength);

	void AsyncSend ();
	void Send (const CBitOutStream& _rkOutStream, const udp::endpoint& _rkEndPoint);

private:
	udp::socket m_kSocket;
	udp::endpoint m_kEndpoint;

	std::deque<SSendCommand> m_kSendQueue;
	std::array<uint8_t, UDP_SOCKET_BUFFER_SIZE> m_kReceiveBuffer;
};