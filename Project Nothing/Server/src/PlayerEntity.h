#pragma once

class CPlayerEntity : public INetEntity
{
public:
	CPlayerEntity ();
	virtual ~CPlayerEntity ();

	inline bool IsInGame () const { return m_nGameID != 0; };

private:
	uint32_t m_nGameID;
};