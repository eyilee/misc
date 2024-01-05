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

		SSendCommand (const std::vector<uint8_t>& _rkBytes);
		virtual ~SSendCommand ();
	};

public:
	CUdpSession (udp::socket& _rkSocket);
	virtual ~CUdpSession ();

	inline udp::endpoint GetEndpoint () const { return m_kSocket.remote_endpoint (); }

	void Init (std::shared_ptr<CUdpConnection> _pkUdpConnection);
	void Shutdown ();

private:
	void AsyncReceive ();
	void OnReceive (size_t _nLength);

	void AsyncSend ();
	void Send (const CBitOutStream& _rkOutStream);

private:
	std::shared_ptr<CUdpConnection> m_pkUdpConnection;
	udp::socket m_kSocket;

	std::deque<SSendCommand> m_kSendQueue;
	std::array<uint8_t, UDP_SOCKET_BUFFER_SIZE> m_kReceiveBuffer;
};