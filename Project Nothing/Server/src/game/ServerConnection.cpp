#include "stdafx.h"
#include "PlayerEntity.h"
#include "game/ServerGame.h"
#include "game/ServerConnection.h"

SGameOutPacket::SGameOutPacket ()
	: m_nMessage (EGameMessage::None)
{
}

SGameOutPacket::~SGameOutPacket ()
{
}

void SGameOutPacket::Reset ()
{
	m_nMessage = EGameMessage::None;
}

CServerConnection::CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession, std::shared_ptr<CServerGame> _pkServerGame, std::shared_ptr<CPlayerEntity> _pkPlayerEntity)
	: CUdpConnection<SGameOutPacket> (_pkUdpSession)
	, m_pkServerGame (_pkServerGame)
	, m_pkPlayerEntity (_pkPlayerEntity)
	, m_nInCommandSequence (0)
	, m_nProcessedCommandSequence (0)
{
}

CServerConnection::~CServerConnection ()
{
}

uint32_t CServerConnection::GetPlayerID ()
{
	if (m_pkPlayerEntity == nullptr) {
		return 0;
	}

	return m_pkPlayerEntity->GetID ();
}

void CServerConnection::ProcessCommands (uint32_t _nTick)
{
	for (int sequence = m_nProcessedCommandSequence + 1; sequence <= m_nInCommandSequence; sequence++)
	{
		SUserCommand* command = m_kInCommands.TryGet (sequence);
		if (command != nullptr && command->m_nTick <= _nTick)
		{
			m_pkServerGame->ProcessCommand (m_pkPlayerEntity->GetID (), *command, command->m_nTick);
			m_nProcessedCommandSequence = sequence;

			m_kInCommands.Remove (sequence);
		}
	}
}

void CServerConnection::ResolvePackage (CBitInStream& _rkInStream)
{
	int message;
	_rkInStream.Read (message);

	if ((message & static_cast<int> (EGameMessage::Command)) > 0) {
		ResolveCommand (_rkInStream);
	}
}

void CServerConnection::ResolveCommand (CBitInStream& _rkInStream)
{
	int sequence;
	_rkInStream.Read (sequence);

	if (sequence > m_nInCommandSequence) {
		m_nInCommandSequence = sequence;
	}

	while (true)
	{
		uint8_t hasCommand;
		_rkInStream.Read (hasCommand);

		if (hasCommand == 0) {
			break;
		}

		SUserCommand& command = m_kInCommands.Insert (sequence);
		_rkInStream.Read (command);

		sequence--;
	}
}

void CServerConnection::OnPacketAcked (int _nSequence, SGameOutPacket& _rkOutPacket)
{
	_rkOutPacket.Reset ();
}

void CServerConnection::ComposePackage ()
{
	if (!CanComposeOutput ()) {
		return;
	}

	SGameOutPacket& outPacket = m_kOutPackets.Insert (m_nOutSequence);

	CBitOutStream outStream;
	BeginComposeOutput (outStream);

	int message = 0;
	message |= static_cast<int> (EGameMessage::Snapshot);
	outStream.Write (message);

	// TODO: write snapshot

	EndComposeOutput (outStream);
}
