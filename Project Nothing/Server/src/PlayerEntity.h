#pragma once

class CPlayerEntity : public CEntity<CPlayerEntity>
{
public:
	CPlayerEntity ();
	virtual ~CPlayerEntity ();

	void set_id (int _nID) { m_nID = _nID; }
	int get_id () { return m_nID; }

private:
	int m_nID;
};