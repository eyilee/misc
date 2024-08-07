#include "stdafx.h"
#include "framework/network/UdpSession.h"
#include "framework/network/UdpConnection.h"

SFragmentHeader::SFragmentHeader ()
	: m_nSequence (0)
	, m_nCount (0)
	, m_nIndex (0)
	, m_nSize (0)
{
}

SFragmentHeader::~SFragmentHeader ()
{
}

void SFragmentHeader::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nSequence);
	_rkOutStream.Write (m_nCount);
	_rkOutStream.Write (m_nIndex);
	_rkOutStream.Write (m_nSize);
}

void SFragmentHeader::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nSequence);
	_rkInStream.Read (m_nCount);
	_rkInStream.Read (m_nIndex);
	_rkInStream.Read (m_nSize);
}

SFragmentReassembly::SFragmentReassembly ()
	: m_nCount (0)
	, m_nReceivedMask (0)
	, m_nReceivedCount (0)
{
	m_kBytes.resize (REASSEMBLY_SIZE);
}

SFragmentReassembly::~SFragmentReassembly ()
{
}

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
