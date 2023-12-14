#pragma once

using boost::asio::ip::tcp;

class CBitOutStream;
class CNetBridge;

constexpr size_t TCP_SESSION_BUFFER_SIZE = 8192;

class CTcpSession : public std::enable_shared_from_this<CTcpSession>
{
	friend CNetBridge;

private:
	struct SCommand
	{
		size_t m_nByteOffset;
		std::vector<uint8_t> m_kBytes;

		SCommand (const std::vector<uint8_t>& _rkBytes)
			: m_nByteOffset (0)
		{
			constexpr size_t size = sizeof (unsigned short);
			unsigned short byteCount = static_cast<unsigned short> (_rkBytes.size ());
			m_kBytes.resize (size + _rkBytes.size ());

			auto it = m_kBytes.begin ();
			std::copy ((uint8_t*)&byteCount, (uint8_t*)&byteCount + size, it);
			std::advance (it, size);
			std::copy (_rkBytes.begin (), _rkBytes.end (), it);
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

	void OnRead (const boost::asio::const_buffer& _rkBuffer);
	void OnWrite (const CBitOutStream& _rkOutStream);

private:
	tcp::socket m_kSocket;

	std::deque<SCommand> m_kCommandQueue;
	std::array<uint8_t, TCP_SESSION_BUFFER_SIZE> m_kReadBuffer;
	std::array<uint8_t, TCP_SESSION_BUFFER_SIZE> m_kCommandBuffers[2];

	std::shared_ptr<CNetBridge> m_pkNetBridge;
};