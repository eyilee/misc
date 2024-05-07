#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitStream.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/TcpClient.h"

using boost::asio::ip::tcp;

CTcpClient::CTcpClient (boost::asio::io_context& _rkContext, std::string _kHostAddr, std::string _kPort)
	: m_kResolver (_rkContext)
	, m_kSocket (_rkContext)
	, m_kHostAddr (_kHostAddr)
	, m_kPort (_kPort)
	, m_kWriteBuffer ()
{
}

CTcpClient::~CTcpClient ()
{
}

void CTcpClient::Init ()
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

void CTcpClient::ComposeOutput (std::shared_ptr<INetProtocol> _pkNetProtocol)
{
	CBitOutStream outStream;
	_pkNetProtocol->OnSerialize (outStream);
	OnWrite (outStream);
}

void CTcpClient::AsyncWrite (std::size_t _nBytes)
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

void CTcpClient::OnWrite (const CBitOutStream& _rkOutStream)
{
	const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
	std::copy (bytes.begin (), bytes.end (), m_kWriteBuffer.begin ());

	AsyncWrite (bytes.size ());
}
