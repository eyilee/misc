#pragma once

using boost::asio::ip::tcp;

class INetProtocol;
class COutStream;

class CTcpClient : public std::enable_shared_from_this<CTcpClient>
{
public:
	CTcpClient (boost::asio::io_context& _kIo_context, std::string _kHost, std::string _kPort);
	virtual ~CTcpClient ();

	void init ();

	void compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol);

private:
	void async_write (std::size_t _nLength);

	void on_write (const COutStream& _kOut_stream);

private:
	tcp::resolver m_kResolver;
	tcp::socket m_kSocket;

	std::string m_kHost;
	std::string m_kPort;

	std::array<char, 1024> m_kSend_buffer;
};