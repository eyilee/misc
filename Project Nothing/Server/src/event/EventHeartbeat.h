#pragma once

class CEventHeartbeat : public IEvent
{
public:
	CEventHeartbeat ();
	virtual ~CEventHeartbeat ();

	void Excute () override;

private:
	void SetNextTime ();
};
