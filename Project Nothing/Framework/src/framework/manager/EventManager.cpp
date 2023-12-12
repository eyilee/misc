#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/event/Event.h"
#include "framework/manager/EventManager.h"

CEventManager::CEventManager ()
	: m_pkTimer (nullptr)
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
}

void CEventManager::Shutdown ()
{
	m_kEventList.clear ();

	if (m_pkTimer != nullptr)
	{
		m_pkTimer->cancel ();
		m_pkTimer = nullptr;
	}

	Instance = nullptr;
}

void CEventManager::AddEvent (std::shared_ptr<CEvent> _pkEvent)
{
	long long time = _pkEvent->GetTime ();

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
	auto time = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now ().time_since_epoch ()).count ();
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
