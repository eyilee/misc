#pragma once
#include "game/GameWorld.h"

struct SUserCommand;
class CPlayerEntity;
class CServerConnection;

constexpr int SNAPSHOT_BUFFER_SIZE = 128;
constexpr int SNAPSHOT_DATA_SIZE = 1024 * 64;

class CServerGame : public IGameLoop, public std::enable_shared_from_this<CServerGame>
{
	enum class EGameState
	{
		Idle,
		Loading,
		Active
	};

	struct SGameObjectSnapshotInfo
	{
	public:
		uint8_t* m_pnData;
		size_t m_nLength;
	};

	struct SGameObjectInfo
	{
	public:
		uint32_t m_nType;
		uint32_t m_nSpawnSequence;
		uint32_t m_nDepawnSequence;
		uint32_t m_nUpdateSequence;
		SequenceBuffer<SGameObjectSnapshotInfo, SNAPSHOT_BUFFER_SIZE> m_kSnapshots;
	};

	struct SWorldSnapshotInfo
	{
	public:
		uint32_t m_nTick;
		std::array<uint8_t, SNAPSHOT_DATA_SIZE> m_kData;
		size_t m_nLength;
	};

public:
	CServerGame ();
	virtual ~CServerGame ();

	virtual void Init () override;
	virtual void Shutdown () override;

	virtual void Update () override;

	bool Join (std::shared_ptr<CPlayerEntity> _pkPlayerEntity, std::shared_ptr<CUdpSession> _pkUdpSession);
	void Leave (std::shared_ptr<CPlayerEntity> _pkPlayerEntity);

	void ProcessCommand (uint32_t _nPlayerID, const SUserCommand& _rkCommand, uint32_t _nTick);

private:
	void UpdateLoadingState ();

	void EnterActiveState ();
	void UpdateActiveState ();
	void LeaveActiveState ();

	void OnJoin (std::shared_ptr<CServerConnection> _pkConnection);
	void OnLeave (std::shared_ptr<CServerConnection> _pkConnection);

	void TickUpdate ();
	void GenerateSnapshot ();
	void BroadcastSnapshot ();

public:
	static unsigned short ServerTickRate;
	static unsigned short ClientTickRate;
	static uint32_t TickStep;
	static uint64_t TickInterval;

private:
	CStateMachine<EGameState> m_kStateMachine;

	uint32_t m_nTick;
	uint64_t m_nNextTickTime;

	std::map<uint32_t, std::shared_ptr<CServerConnection>> m_kServerConnections;

	uint32_t m_nServerSequence;
	std::vector<SGameObjectInfo> m_kGameObjectInfos;
	std::vector<uint32_t> m_kFreeGameObjectInfos;

	std::array<SWorldSnapshotInfo, SNAPSHOT_BUFFER_SIZE> m_kSnapshots;

	CGameWorld m_kGameWorld;
};