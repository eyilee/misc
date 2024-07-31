#pragma once

class INetProtocol;
class CBitOutStream;

constexpr size_t TCP_CLIENT_BUFFER_SIZE = 8192;

class CTcpClient : public std::enable_shared_from_this<CTcpClient>
{
public:
	CTcpClient (boost::asio::io_context& _rkContext, std::string _rkHostAddr, std::string _rkPort);
	virtual ~CTcpClient ();

	void Init ();

	void ComposeOutput (std::shared_ptr<INetProtocol> _pkNetProtocol);

private:
	void AsyncWrite (std::size_t _nLength);

	void OnWrite (const CBitOutStream& _rkOutStream);

private:
	boost::asio::ip::tcp::resolver m_kResolver;
	boost::asio::ip::tcp::socket m_kSocket;

	std::string m_kHostAddr;
	std::string m_kPort;

	std::array<char, TCP_CLIENT_BUFFER_SIZE> m_kWriteBuffer;
};
