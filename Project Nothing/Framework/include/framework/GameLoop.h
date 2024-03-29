#pragma once

class IGameLoop
{
public:
	IGameLoop ();
	virtual ~IGameLoop ();

	inline uint32_t GetID () const { return m_nID; }
	inline void SetID (uint32_t _nID) { m_nID = _nID; }

	virtual void Init () = 0;
	virtual void Shutdown () = 0;

	virtual void Update () = 0;

protected:
	uint32_t m_nID;
};
