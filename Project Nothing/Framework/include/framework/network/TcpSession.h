#pragma once
#include "framework/network/BitStream.h"

using boost::asio::ip::tcp;

class CTcpConnection;

constexpr size_t TCP_SOCKET_BUFFER_SIZE = 8192;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
	friend CTcpConnection;

private:
	struct SReadCommand
	{
		size_t m_nHeaderOffset;
		std::vector<uint8_t> m_kHeader;

		size_t m_nByteOffset;
		std::vector<uint8_t> m_kBytes;

		SReadCommand ()
			: m_nHeaderOffset (0)
			, m_nByteOffset (0)
		{
			m_kHeader.resize (2);
		}
	};

	struct SWriteCommand
	{
		size_t m_nByteOffset;
		std::vector<uint8_t> m_kBytes;

		SWriteCommand (CBitOutStream& _rkOutStream)
			: m_nByteOffset (0)
		{
			const std::vector<uint8_t>& header = _rkOutStream.GetHeader ();
			const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
			m_kBytes.resize (header.size () + bytes.size ());
			std::copy (header.begin (), header.end (), m_kBytes.begin ());
			std::copy (bytes.begin (), bytes.end (), m_kBytes.begin () + header.size ());
		}
	};

public:
	CTcpSession (tcp::socket& _rkSocket);
	virtual ~CTcpSession ();

	void Init (std::shared_ptr<CTcpConnection> _pkTcpConnection);
	void Shutdown ();

	inline uint32_t GetIP () const { return m_kSocket.remote_endpoint ().address ().to_v4 ().to_uint (); }

private:
	void AsyncRead ();
	void OnRead (const size_t& _rnLength);

	void AsyncWrite ();
	void Write (CBitOutStream& _rkOutStream);

private:
	std::shared_ptr<CTcpConnection> m_pkTcpConnection;
	tcp::socket m_kSocket;

	std::deque<SReadCommand> m_kReadQueue;
	std::deque<SWriteCommand> m_kWriteQueue;
	std::array<uint8_t, TCP_SOCKET_BUFFER_SIZE> m_kReadBuffer;
};