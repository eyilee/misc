#pragma once

class CPlayerEntity : public CEntity<CPlayerEntity>
{
public:
	CPlayerEntity ();
	virtual ~CPlayerEntity ();

	void set_id (const int& _nId) { m_nId = _nId; }
	int get_id () { return m_nId; }

private:
	int m_nId;
};