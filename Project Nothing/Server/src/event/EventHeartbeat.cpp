#include "stdafx.h"
#include "EventHeartbeat.h"

CEventHeartbeat::CEventHeartbeat ()
{
	SetNextTime ();
}

CEventHeartbeat::~CEventHeartbeat ()
{
}

void CEventHeartbeat::Excute ()
{
	LOG_INFO ("Heart Beat.");
	SetNextTime ();
}

void CEventHeartbeat::SetNextTime ()
{
	m_nTime = CTime::GetMiliSecond () + 1000;
}
