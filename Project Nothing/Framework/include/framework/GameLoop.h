#pragma once

class IGameLoop
{
public:
	IGameLoop ();
	virtual ~IGameLoop ();

	inline uint32_t GetID () const { return m_nID; }
	inline void SetID (uint32_t _nID) { m_nID = _nID; }

	inline bool IsRunning () const { return m_bIsRunning; }
	inline void SetRunning (bool _bIsRunning) { m_bIsRunning = _bIsRunning; }

	virtual void Init () = 0;
	virtual void Shutdown () = 0;

	virtual void Update () = 0;

protected:
	uint32_t m_nID;
	bool m_bIsRunning;
};
