#include "stdafx.h"
#include "EventHeartbeat.h"

CEventHeartbeat::CEventHeartbeat ()
{
	auto now = std::chrono::system_clock::now ();
	now += std::chrono::seconds (5);
	m_nTime = std::chrono::duration_cast<std::chrono::milliseconds> (now.time_since_epoch ()).count ();
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
