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

	inline unsigned short GetPort () const { return m_kSocket.local_endpoint ().port (); }
	inline uint32_t GetKey () const { return m_nKey; }

	void Init (std::shared_ptr<CUdpConnection> _pkUdpConnection);
	void Shutdown ();

private:
	void AsyncReceive ();
	void OnReceive (size_t _nLength);

	void AsyncSend ();
	void Send (const CBitOutStream& _rkOutStream);

private:
	std::shared_ptr<CUdpConnection> m_pkUdpConnection;
	uint32_t m_nKey;
	udp::socket m_kSocket;

	std::deque<SSendCommand> m_kSendQueue;
	std::array<uint8_t, UDP_SOCKET_BUFFER_SIZE> m_kReceiveBuffer;
};