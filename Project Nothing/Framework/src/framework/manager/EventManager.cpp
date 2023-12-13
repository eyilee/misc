#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/Event.h"
#include "framework/Time.h"
#include "framework/manager/EventManager.h"

CEventManager::CEventManager ()
	: m_bIsRunning (false)
	, m_kInterval (boost::posix_time::milliseconds (100))
	, m_pkTimer (nullptr)
{
}

CEventManager::~CEventManager ()
{
}

void CEventManager::Init (boost::asio::io_context& _rkContext, const unsigned short& _rnEventRate)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CEventManager> ();
	Instance->Run (_rkContext, _rnEventRate);
}

void CEventManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Stop ();
	Instance = nullptr;
}

void CEventManager::PushEvent (std::shared_ptr<CEvent> _pkEvent)
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Queue (_pkEvent);
}

void CEventManager::Run (boost::asio::io_context& _rkContext, const unsigned short& _rnEventRate)
{
	m_bIsRunning = true;

	m_kInterval = boost::posix_time::milliseconds (1000 / _rnEventRate);

	auto self (shared_from_this ());
	m_fnTick = [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (!m_bIsRunning)
			{
				m_pkTimer->cancel ();
				m_pkTimer = nullptr;
			}
			else
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
			}
		};

	m_pkTimer = std::make_shared<boost::asio::deadline_timer> (_rkContext);
	m_pkTimer->expires_from_now (m_kInterval);
	m_pkTimer->async_wait (m_fnTick);
}

void CEventManager::Stop ()
{
	m_bIsRunning = false;
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

				if (latest->IsValid () && time <= latest->GetTime ()) {
					Queue (latest);
				}
			}
			else {
				break;
			}
		}
		else {
			m_kEventList.pop_front ();
		}
	}

	Flush ();
}

void CEventManager::Queue (std::shared_ptr<CEvent> _pkEvent)
{
	m_kEventQueue.emplace_front (_pkEvent);
}

void CEventManager::Flush ()
{
	uint64_t time = CTime::GetMiliSecond ();

	std::for_each (m_kEventQueue.begin (), m_kEventQueue.end (), [this, &time](std::shared_ptr<CEvent> _pkEvent)
		{
			auto pos = m_kEventList.before_begin ();
			for (auto it = m_kEventList.begin (); it != m_kEventList.end (); it++)
			{
				pos = it;

				if (time > (*it)->GetTime ()) {
					break;
				}
			}

			m_kEventList.emplace_after (pos, _pkEvent);
		});

	m_kEventQueue.clear ();
}
