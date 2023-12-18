#include "stdafx.h"
#include "protocol/command/NC_ClientEchoResult.h"
#include "protocol/event/NE_ServerEcho.h"

NE_ServerEcho::NE_ServerEcho ()
{
}

NE_ServerEcho::~NE_ServerEcho ()
{
}

void NE_ServerEcho::Serialize (CBitOutStream& _rkOutStream)
{
}

void NE_ServerEcho::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_kString);
}

void NE_ServerEcho::Excute ()
{
	std::shared_ptr<INetProtocol> protocol = std::make_shared<NC_ClientEchoResult> (m_kString);
	m_pkNetBridge->ComposeTcpOutput (protocol);

	std::shared_ptr<IEntity> entity = m_pkNetBridge->GetEntity ();
	if (entity == nullptr) {
		LOG_ERROR ("Entity not found.");
	}
}
