#include "stdafx.h"
#include "PlayerEntity.h"
#include "game/ServerGame.h"
#include "game/ServerConnection.h"

CServerConnection::CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession, std::shared_ptr<CServerGame> _pkServerGame, std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
	: CUdpConnection (_pkUdpSession)
	, m_pkServerGame (_pkServerGame)
	, m_pkPlayerEntity (_pkPlayerEntity)
{
}

CServerConnection::~CServerConnection ()
{
}

uint32_t CServerConnection::GetPlayerID ()
{
	if (m_pkPlayerEntity != nullptr) {
		return m_pkPlayerEntity->GetID ();
	}

	return 0;
}

void CServerConnection::ResolvePackage (CBitInStream& _rkInStream)
{
	unsigned short protocolID;
	_rkInStream.Read (protocolID);

	std::shared_ptr<INetProtocol> protocol = CProtocolManager::GenerateProtocol (protocolID);
	if (protocol != nullptr)
	{
		protocol->Deserialize (_rkInStream);
		protocol->Excute ();
	}
}

void CServerConnection::OnPacketAcked (uint32_t _nSequence, SOutPacket& _rkOutPacket)
{
	_rkOutPacket.Reset ();
}
