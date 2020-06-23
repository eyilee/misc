#include "stdafx.h"

#include "network/CNetProtocol.h"
#include "network/COutStream.h"
#include "network/CTcpClient.h"

CTcpClient::CTcpClient (boost::asio::io_context& _kIo_context, std::string _kHost, std::string _kPort)
    : m_kResolver (_kIo_context)
    , m_kSocket (_kIo_context)
    , m_kHost (_kHost)
    , m_kPort (_kPort)
    , m_kSend_buffer ()
{
}

CTcpClient::~CTcpClient ()
{
}

void CTcpClient::init ()
{
	boost::asio::async_connect (m_kSocket, m_kResolver.resolve (m_kHost, m_kPort),
		[this](boost::system::error_code error, tcp::endpoint)
		{
			if (error) {
				std::cout << error.message () << std::endl;
			}
		});
}

void CTcpClient::compose_output (std::shared_ptr<INetProtocol>& _pNet_protocol)
{
	COutStream kOut_Stream;
	_pNet_protocol->on_serialize (kOut_Stream);
	on_write (kOut_Stream);
}

void CTcpClient::async_write (std::size_t _nLength)
{
    auto self (shared_from_this ());
    boost::asio::async_write (m_kSocket, boost::asio::buffer (m_kSend_buffer, _nLength),
        [this, self](boost::system::error_code error, std::size_t /*length*/)
        {
            if (error) {
                std::cout << error.message () << std::endl;
            }
        });
}

void CTcpClient::on_write (const COutStream& _kOut_stream)
{
    const std::vector<char>& kData = _kOut_stream.data ();
    std::copy (kData.begin (), kData.end (), m_kSend_buffer.begin ());

    async_write (kData.size ());
}
