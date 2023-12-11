#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitStream.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpClient.h"

CTcpClient::CTcpClient (boost::asio::io_context& _kIo_context, std::string _kHost, std::string _kPort)
	: m_kResolver (_kIo_context)
	, m_kSocket (_kIo_context)
	, m_kHostAddr (_kHost)
	, m_kPort (_kPort)
	, m_kWriteBuffer ()
{
}

CTcpClient::~CTcpClient ()
{
}

void CTcpClient::init ()
{
	auto self (shared_from_this ());
	boost::asio::async_connect (m_kSocket, m_kResolver.resolve (m_kHostAddr, m_kPort),
		[this, self](const boost::system::error_code& _rkErrorCode, tcp::endpoint _kEndPoint)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
		});
}

void CTcpClient::compose_output (std::shared_ptr<INetProtocol> _pkNetProtocol)
{
	CBitOutStream outStream;
	_pkNetProtocol->on_serialize (outStream);
	on_write (outStream);
}

void CTcpClient::async_write (std::size_t _nBytes)
{
	auto self (shared_from_this ());
	boost::asio::async_write (m_kSocket, boost::asio::buffer (m_kWriteBuffer, _nBytes),
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
		});
}

void CTcpClient::on_write (const CBitOutStream& _rkOutStream)
{
	const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
	std::copy (bytes.begin (), bytes.end (), m_kWriteBuffer.begin ());

	async_write (bytes.size ());
}
