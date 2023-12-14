#include "stdafx.h"
#include "protocol/client/ClientUdpConnectResult.h"
#include "protocol/server/ServerUdpConnect.h"

ServerUdpConnect::ServerUdpConnect ()
{
}

ServerUdpConnect::~ServerUdpConnect ()
{
}

void ServerUdpConnect::Serialize (CBitOutStream& _rkOutStream)
{
}

void ServerUdpConnect::Deserialize (CBitInStream& _rkInStream)
{
}

void ServerUdpConnect::Excute ()
{
	m_pkNetBridge->SetUdpEndPoint ();

	std::shared_ptr<INetProtocol> protocol = std::make_shared<ClientUdpConnectResult> ();
	LOG_DEBUG ("Protocol: %hu.", ClientUdpConnectResult::GetID ());
	m_pkNetBridge->ComposeTcpOutput (protocol);
}
