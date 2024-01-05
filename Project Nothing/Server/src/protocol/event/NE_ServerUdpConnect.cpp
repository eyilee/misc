#include "stdafx.h"
#include "protocol/command/NC_ClientUdpConnectResult.h"
#include "protocol/event/NE_ServerUdpConnect.h"

NE_ServerUdpConnect::NE_ServerUdpConnect ()
	: m_nPort (0)
{
}

NE_ServerUdpConnect::~NE_ServerUdpConnect ()
{
}

void NE_ServerUdpConnect::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nPort);
}

void NE_ServerUdpConnect::Excute ()
{
	CNetworkManager::UdpConnect (m_pkNetBridge, m_nPort);

	bool result = false;
	unsigned short port = 0;

	std::shared_ptr<CUdpConnection> udpConnection = m_pkNetBridge->GetUdpConnection ();
	if (udpConnection != nullptr)
	{
		result = true;
		port = udpConnection->GetLocalEndpoint ().port ();
	}

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientUdpConnectResult> (result, port);
	m_pkNetBridge->ComposeTcpOutput (protocol);
}
