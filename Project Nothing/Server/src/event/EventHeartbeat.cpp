#include "stdafx.h"
#include "EventHeartbeat.h"

CEventHeartbeat::CEventHeartbeat ()
{
	m_nTime = CTime::GetMiliSecond () + 1000;
}

CEventHeartbeat::~CEventHeartbeat ()
{
}

void CEventHeartbeat::Excute ()
{
	LOG_INFO ("heart beat.");

	if (CEventManager::Instance != nullptr) {
		CEventManager::Instance->AddEvent (std::make_shared<CEventHeartbeat> ());
	}

	m_bIsValid = false;
}
