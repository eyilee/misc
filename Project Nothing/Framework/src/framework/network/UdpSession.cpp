#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/network/BitStream.h"
#include "framework/network/UdpSession.h"

CUdpListener::CUdpListener (boost::asio::io_context& _rkContext, const std::string& _rkHostAddr, short _nPort)
	: m_kSocket (_rkContext, udp::endpoint (boost::asio::ip::address::from_string (_rkHostAddr.c_str ()), _nPort))
	, m_kSendBuffer {}
	, m_kReceiveBuffer {}
{
}

CUdpListener::~CUdpListener ()
{
}

void CUdpListener::init ()
{
	async_receive ();
}

void CUdpListener::shutdown ()
{
	auto self (shared_from_this ());
	m_kSocket.async_wait (udp::socket::wait_read, [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}

			m_kSocket.shutdown (udp::socket::shutdown_type::shutdown_both);
			m_kSocket.close ();
		});
}

void CUdpListener::async_receive ()
{
	auto self (shared_from_this ());
	m_kSocket.async_receive_from (boost::asio::buffer (m_kReceiveBuffer, UDP_SESSION_BUFFER_SIZE), m_kEndpoint,
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
			else {
				on_receive (boost::asio::buffer (m_kReceiveBuffer, _nLength));
			}

			async_receive ();
		});
}

void CUdpListener::async_send (std::size_t _nBytes)
{
	auto self (shared_from_this ());
	m_kSocket.async_send_to (boost::asio::buffer (m_kSendBuffer, _nBytes), m_kEndpoint,
		[this, self](const boost::system::error_code& _rkErrorCode, std::size_t _nLength)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
		});
}

void CUdpListener::on_receive (const boost::asio::const_buffer& _rkBuffer)
{
	const uint8_t* buffer = boost::asio::buffer_cast<const uint8_t*> (_rkBuffer);
	CBitInStream inStream (buffer, _rkBuffer.size ());

	uint32_t ip = m_kEndpoint.address ().to_v4 ().to_uint ();

	// m_pkNetBridge->resolve_input (inStream);
}

void CUdpListener::on_send (const CBitOutStream& _rkOutStream)
{
	const std::vector<uint8_t>& bytes = _rkOutStream.GetBytes ();
	std::copy (bytes.begin (), bytes.end (), m_kSendBuffer.begin ());

	async_send (bytes.size ());
}
