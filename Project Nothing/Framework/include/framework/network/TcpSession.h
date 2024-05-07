#pragma once
#include "framework/network/BitStream.h"

class CTcpConnection;

constexpr size_t TCP_SOCKET_BUFFER_SIZE = 8192;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
private:
	struct SReadCommand
	{
		size_t m_nHeaderOffset;
		std::vector<uint8_t> m_kHeader;

		size_t m_nByteOffset;
		std::vector<uint8_t> m_kBytes;

		SReadCommand ();
		virtual ~SReadCommand ();
	};

	struct SWriteCommand
	{
		size_t m_nByteOffset;
		std::vector<uint8_t> m_kBytes;

		SWriteCommand (CBitOutStream& _rkOutStream);
		virtual ~SWriteCommand ();
	};

public:
	CTcpSession (boost::asio::ip::tcp::socket& _rkSocket);
	virtual ~CTcpSession ();

	inline boost::asio::ip::tcp::endpoint GetRemoteEndpoint () const { return m_kSocket.remote_endpoint (); }

	void Init (std::shared_ptr<CTcpConnection> _pkTcpConnection);
	void Shutdown ();

	void Write (CBitOutStream& _rkOutStream);

private:
	void AsyncRead ();
	void OnRead (const size_t& _rnLength);

	void AsyncWrite ();

private:
	std::shared_ptr<CTcpConnection> m_pkConnection;
	boost::asio::ip::tcp::socket m_kSocket;

	std::deque<SReadCommand> m_kReadQueue;
	std::deque<SWriteCommand> m_kWriteQueue;
	std::array<uint8_t, TCP_SOCKET_BUFFER_SIZE> m_kReadBuffer;
};