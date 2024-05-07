#pragma once

class CPlayerEntity : public INetEntity, public std::enable_shared_from_this<CPlayerEntity>
{
public:
	CPlayerEntity ();
	virtual ~CPlayerEntity ();

	virtual void OnConnected () override;
	virtual void OnDisconnected () override;

	inline uint32_t GetGameID () const { return m_nGameID; }
	inline void SetGameID (uint32_t _nGameID) { m_nGameID = _nGameID; }

private:
	uint32_t m_nGameID;
};
