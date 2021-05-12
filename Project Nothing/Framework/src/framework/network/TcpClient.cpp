#include "stdafx.h"

#include "framework/network/NetProtocol.h"
#include "framework/network/OutStream.h"
#include "framework/network/TcpClient.h"

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
	auto self (shared_from_this ());
	boost::asio::async_connect (m_kSocket, m_kResolver.resolve (m_kHost, m_kPort),
		[this, self](const boost::system::error_code& _kError_code, tcp::endpoint)
		{
			if (_kError_code) {
				LOG_ERROR (_kError_code.message ());
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
		[this, self](const boost::system::error_code& _kError_code, std::size_t)
		{
			if (_kError_code) {
				LOG_ERROR (_kError_code.message ());
			}
		});
}

void CTcpClient::on_write (const COutStream& _kOut_stream)
{
	const std::vector<char>& kData = _kOut_stream.data ();
	std::copy (kData.begin (), kData.end (), m_kSend_buffer.begin ());

	async_write (kData.size ());
}
