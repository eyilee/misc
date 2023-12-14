#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::tcp;

class CNetBridge;

constexpr size_t TCP_SOCKET_BUFFER_SIZE = 8192;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
	friend CNetBridge;

private:
	struct SReadCommand
	{
		size_t m_nSize;
		std::vector<uint8_t> m_kBytes;

		SReadCommand (const size_t& _rnSize)
		{
			m_nSize = _rnSize;
			m_kBytes.reserve (m_nSize);
		}
	};

	struct SWriteCommand
	{
		size_t m_nByteOffset;
		std::vector<uint8_t> m_kBytes;

		SWriteCommand (const CBitOutStream& _rkOutStream)
			: m_nByteOffset (0)
		{
			std::vector<uint8_t> header = _rkOutStream.GetHeader ();
			const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
			m_kBytes.reserve (header.size () + bytes.size ());
			std::copy (header.begin (), header.end (), m_kBytes.end ());
			std::copy (bytes.begin (), bytes.end (), m_kBytes.end ());
		}
	};

public:
	CTcpSession (tcp::socket& _rkSocket);
	virtual ~CTcpSession ();

	void Init ();
	void Shutdown ();

private:
	void AsyncRead ();
	void AsyncWrite ();

	void OnRead (const size_t& _rnLength);
	void OnWrite (const CBitOutStream& _rkOutStream);

private:
	tcp::socket m_kSocket;

	std::deque<SReadCommand> m_kBufferQueue;
	std::deque<SWriteCommand> m_kWriteQueue;
	std::array<uint8_t, TCP_SOCKET_BUFFER_SIZE> m_kReadBuffer;

	std::shared_ptr<CNetBridge> m_pkNetBridge;
};