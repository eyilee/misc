#include "stdafx.h"
#include "protocol/netcommand/NC_ClientUdpConnectResult.h"

NC_ClientUdpConnectResult::NC_ClientUdpConnectResult ()
{
}

NC_ClientUdpConnectResult::NC_ClientUdpConnectResult (bool _bResult, unsigned short _nPort)
	: m_bResult (_bResult)
	, m_nPort (_nPort)
{
}

NC_ClientUdpConnectResult::~NC_ClientUdpConnectResult ()
{
}

void NC_ClientUdpConnectResult::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_bResult);
	_rkOutStream.Write (m_nPort);
}
