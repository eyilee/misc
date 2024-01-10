#pragma once
#include "game/GameWorld.h"

class CPlayerEntity;
class CServerConnection;

class CServerGame : public IGameLoop, public std::enable_shared_from_this<CServerGame>
{
	enum class EGameState
	{
		Idle,
		Loading,
		Active
	};

	struct SGameObjectInfo
	{
		uint32_t m_nType;

	};

public:
	CServerGame ();
	virtual ~CServerGame ();

	virtual void Init () override;
	virtual void Shutdown () override;

	virtual void Update () override;

	bool Join (std::shared_ptr<CPlayerEntity> _pkPlayerEntity, std::shared_ptr<CUdpSession> _pkUdpSession);
	void Leave (std::shared_ptr<CPlayerEntity> _pkPlayerEntity);

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

	std::vector<SGameObjectInfo> m_kGameObjectInfos;
	std::vector<uint32_t> m_kFreeGameObjectInfos;

	CGameWorld m_kGameWorld;
};