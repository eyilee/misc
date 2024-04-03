#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::udp;

class IUdpConnection;

constexpr size_t UDP_SOCKET_BUFFER_SIZE = 1432; // 1500(Ethernet MTU) - 60(IPv4 header) - 8(udp header)

class CUdpSession : public std::enable_shared_from_this<CUdpSession>
{
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

	inline unsigned short GetPort () const { return m_kSocket.local_endpoint ().port (); }
	inline uint32_t GetKey () const { return m_nKey; }

	void Init (std::shared_ptr<IUdpConnection> _pkUdpConnection);
	void Shutdown ();

	void Send (const CBitOutStream& _rkOutStream);

private:
	void AsyncReceive ();
	void OnReceive (size_t _nLength);

	void AsyncSend ();

private:
	std::shared_ptr<IUdpConnection> m_pkConnection;
	uint32_t m_nKey;
	udp::socket m_kSocket;

	std::deque<SSendCommand> m_kSendQueue;
	std::array<uint8_t, UDP_SOCKET_BUFFER_SIZE> m_kReceiveBuffer;
};