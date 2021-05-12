#include "stdafx.h"

#include "EventHeartbeat.h"

CEventHeartbeat::CEventHeartbeat ()
{
	auto time = std::chrono::system_clock::now ();
	time += std::chrono::seconds (5);
	m_nTime = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch ()).count ();
}

CEventHeartbeat::~CEventHeartbeat ()
{
}

void CEventHeartbeat::excute ()
{
	LOG_INFO ("heart beat.");

	CEventManager::Instance->add_event (std::make_shared<CEventHeartbeat> ());

	m_bIs_valid = false;
}
