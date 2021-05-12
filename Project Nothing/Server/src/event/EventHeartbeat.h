#pragma once

class CEventHeartbeat : public CEvent
{
public:
	CEventHeartbeat ();
	virtual ~CEventHeartbeat ();

	void excute () override;
};