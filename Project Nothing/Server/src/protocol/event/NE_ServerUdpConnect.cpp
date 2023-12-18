#include "stdafx.h"
#include "protocol/command/NC_ClientUdpConnectResult.h"
#include "protocol/event/NE_ServerUdpConnect.h"

NE_ServerUdpConnect::NE_ServerUdpConnect ()
{
}

NE_ServerUdpConnect::~NE_ServerUdpConnect ()
{
}

void NE_ServerUdpConnect::Serialize (CBitOutStream& _rkOutStream)
{
}

void NE_ServerUdpConnect::Deserialize (CBitInStream& _rkInStream)
{
}

void NE_ServerUdpConnect::Excute ()
{
	m_pkNetBridge->SetUdpEndPoint ();

	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientUdpConnectResult> ();
	m_pkNetBridge->ComposeTcpOutput (protocol);
}
