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

	void Join ();

private:
	void UpdateLoadingState ();

	void EnterActiveState ();
	void UpdateActiveState ();
	void LeaveActiveState ();

	void TickUpdate ();

public:
	static unsigned short ServerTickRate;
	static unsigned short ClientTickRate;
	static uint32_t TickStep;
	static uint64_t TickInterval;

private:
	uint32_t m_nTick;
	uint64_t m_nNextTickTime;
	CStateMachine<EGameState> m_kStateMachine;
};