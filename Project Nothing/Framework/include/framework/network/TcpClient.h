#pragma once

using boost::asio::ip::tcp;

class CBitOutStream;
class INetProtocol;

constexpr size_t TCP_CLIENT_BUFFER_SIZE = 8192;

class CTcpClient : public std::enable_shared_from_this<CTcpClient>
{
public:
	CTcpClient (boost::asio::io_context& _rkContext, std::string _rkHostAddr, std::string _rkPort);
	virtual ~CTcpClient ();

	void init ();

	void compose_output (std::shared_ptr<INetProtocol> _pkNetProtocol);

private:
	void async_write (std::size_t _nLength);

	void on_write (const CBitOutStream& _rkOutStream);

private:
	tcp::resolver m_kResolver;
	tcp::socket m_kSocket;

	std::string m_kHostAddr;
	std::string m_kPort;

	std::array<char, TCP_CLIENT_BUFFER_SIZE> m_kWriteBuffer;
};