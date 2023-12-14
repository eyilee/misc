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
	SetNextTime ();
}

void CEventHeartbeat::SetNextTime ()
{
	m_nTime = CTime::GetMiliSecond () + 1000;
}
