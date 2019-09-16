#pragma once

using asio::ip::tcp;

class CConnection : public std::enable_shared_from_this<CConnection>
{
public:
	CConnection (tcp::socket _socket);
	virtual ~CConnection ();

	void Init ();

private:
	void do_read ();

	void do_write (std::size_t length);

private:
	tcp::socket m_kSocket;

	std::array<char, 1024> m_kSend_buffer;
	std::array<char, 1024> m_kReceive_buffer;
};