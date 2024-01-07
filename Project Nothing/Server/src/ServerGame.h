#pragma once

enum class EGameState
{
	Idle,
	Loading,
	Active
};

class CServerGame : public IGameLoop
{
public:
	CServerGame ();
	virtual ~CServerGame ();

	virtual void Init () override;
	virtual void Shutdown () override;

	virtual void Update () override;
	virtual void LateUpdate () override;

private:
	void UpdateLoadingState ();

	void EnterActiveState ();
	void UpdateActiveState ();
	void LeaveActiveState ();

public:
	static unsigned short ServerTickRate;
	static unsigned short ClientTickRate;
	static uint64_t TickStep;
	static uint64_t TickDuration;

private:
	uint64_t Tick;
	CStateMachine<EGameState> m_kStateMachine;
};