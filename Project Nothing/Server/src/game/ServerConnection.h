#pragma once

struct SUserCommand;
class CServerGame;
class CPlayerEntity;

constexpr uint32_t COMMAND_BUFFER_SIZE = 32;

class CServerConnection : public CUdpConnection
{
public:
	CServerConnection (std::shared_ptr<CUdpSession> _pkUdpSession, std::shared_ptr<CServerGame> _pkServerGame, std::shared_ptr<CPlayerEntity> _pkPlayerEntity);
	virtual ~CServerConnection ();

	uint32_t GetPlayerID ();

	inline std::shared_ptr<CPlayerEntity> GetPlayerEntity () const { return m_pkPlayerEntity; };
	inline void SetPlayerEntity (std::shared_ptr<CPlayerEntity> _pkPlayerEntity) { m_pkPlayerEntity = _pkPlayerEntity; };

	void ProcessCommands (uint32_t _nTick);

protected:
	virtual void ResolvePackage (CBitInStream& _rkInStream) override;
	virtual void OnPacketAcked (uint32_t _nSequence, SOutPacket& _rkOutPacket) override;

protected:
	std::shared_ptr<CServerGame> m_pkServerGame;
	std::shared_ptr<CPlayerEntity> m_pkPlayerEntity;

	uint32_t m_nInCommandSequence;
	uint32_t m_nProcessedCommandSequence;
	SequenceBuffer<SUserCommand, COMMAND_BUFFER_SIZE> m_kOutPackets;
};