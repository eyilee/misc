#include "stdafx.h"
#include "framework/network/UdpSession.h"
#include "framework/network/UdpConnection.h"

SUdpHeader::SUdpHeader ()
	: m_nSequence (0)
	, m_nAck (0)
	, m_nAckBits (0)
{
}

SUdpHeader::~SUdpHeader ()
{
}

void SUdpHeader::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nSequence);
	_rkOutStream.Write (m_nAck);
	_rkOutStream.Write (m_nAckBits);
}

void SUdpHeader::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nSequence);
	_rkInStream.Read (m_nAck);
	_rkInStream.Read (m_nAckBits);
}

IUdpConnection::IUdpConnection ()
{
}

IUdpConnection::~IUdpConnection ()
{
}
