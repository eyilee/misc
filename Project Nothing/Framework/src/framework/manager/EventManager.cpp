#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/time/Time.h"
#include "framework/event/Event.h"
#include "framework/manager/EventManager.h"

CEventManager::CEventManager ()
	: m_bIsRunning (false)
	, m_pkTimer (nullptr)
	, m_kInterval (boost::posix_time::milliseconds (100))
{
}

CEventManager::~CEventManager ()
{
}

void CEventManager::Init (boost::asio::io_context& _rkContext)
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_pkTimer = std::make_shared<boost::asio::deadline_timer> (_rkContext);

	auto self (shared_from_this ());
	m_fnTick = [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (_rkErrorCode) {
				LOG_ERROR (_rkErrorCode.message ());
			}
			else
			{
				if (!m_bIsRunning)
				{
					m_pkTimer->cancel ();
					m_pkTimer = nullptr;
					return;
				}

				Tick ();

				if (m_pkTimer != nullptr)
				{
					m_pkTimer->expires_at (m_pkTimer->expires_at () + m_kInterval);
					m_pkTimer->async_wait (m_fnTick);
				}
			}
		};

	m_pkTimer->expires_from_now (m_kInterval);
	m_pkTimer->async_wait (m_fnTick);
	m_bIsRunning = true;
}

void CEventManager::Shutdown ()
{
	m_bIsRunning = false;
	m_kEventList.clear ();

	Instance = nullptr;
}

void CEventManager::AddEvent (std::shared_ptr<CEvent> _pkEvent)
{
	uint64_t time = _pkEvent->GetTime ();

	auto pos = m_kEventList.before_begin ();
	for (auto it = m_kEventList.begin (); it != m_kEventList.end (); it++)
	{
		pos = it;

		if (time > (*it)->GetTime ()) {
			break;
		}
	}

	m_kEventList.emplace_after (pos, _pkEvent);
}

void CEventManager::Tick ()
{
	uint64_t time = CTime::GetMiliSecond ();
	while (!m_kEventList.empty ())
	{
		std::shared_ptr<CEvent> latest = m_kEventList.front ();
		if (latest->IsValid ())
		{
			if (time >= latest->GetTime ())
			{
				m_kEventList.pop_front ();
				latest->Excute ();
			}
			else {
				break;
			}
		}
		else {
			m_kEventList.pop_front ();
		}
	}
}
