#pragma once

class IGameObject
{
public:
	IGameObject ();
	virtual ~IGameObject ();

	inline uint32_t GetID () const { return m_nID; }
	inline void SetID (uint32_t _nID) { m_nID = _nID; }

	void SetPosition (float _nX, float _nY);

protected:
	uint32_t m_nID;
	float m_nX;
	float m_nY;
};
