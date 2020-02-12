#pragma once

class CDataBaseManager : public CBaseManager<CDataBaseManager>
{
public:
	CDataBaseManager ();
	virtual ~CDataBaseManager ();

	void init ();
};