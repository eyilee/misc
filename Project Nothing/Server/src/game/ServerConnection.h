#pragma once

#include "game/UserCommand.h"

class CServerGame;
class CPlayerEntity;

constexpr size_t COMMAND_BUFFER_SIZE = 32;

enum class EGameMessage : uint32_t
{
	None = 0,
	Command = 1 << 0,
	Snapshot = 1 << 1
};

struct SGameOutPacket
{
public:
	SGameOutPacket ();
	virtual ~SGameOutPacket ();

	void Reset ();

public:
	EGameMessage m_nMessage;
};

class CServerConnection : public CUdpConnection<SGameOutPacket>
{
public:
	CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession, std::shared_ptr<CServerGame> _pkServerGame, std::shared_ptr<CPlayerEntity> _pkPlayerEntity);
	virtual ~CServerConnection ();

	uint32_t GetPlayerID ();

	inline std::shared_ptr<CPlayerEntity> GetPlayerEntity () const { return m_pkPlayerEntity; };
	inline void SetPlayerEntity (std::shared_ptr<CPlayerEntity> _pkPlayerEntity) { m_pkPlayerEntity = _pkPlayerEntity; };

	void ProcessCommands (uint32_t _nTick);

	virtual void ResolvePackage (CBitInStream& _rkInStream) override;
	void ResolveCommand (CBitInStream& _rkInStream);
	virtual void OnPacketAcked (int _nSequence, SGameOutPacket& _rkOutPacket) override;

	void ComposePackage ();

protected:
	std::shared_ptr<CServerGame> m_pkServerGame;
	std::shared_ptr<CPlayerEntity> m_pkPlayerEntity;

	int m_nInCommandSequence;
	int m_nProcessedCommandSequence;
	SequenceBuffer<SUserCommand, COMMAND_BUFFER_SIZE> m_kInCommands;
};
