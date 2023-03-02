#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/event/Event.h"
#include "framework/manager/BaseManager.h"
#include "framework/manager/EventManager.h"

CEventManager::CEventManager ()
	: m_pTimer (nullptr)
	, m_kInterval (boost::posix_time::milliseconds (900))
{
}

CEventManager::~CEventManager ()
{
}

void CEventManager::init (boost::asio::io_context& _kIo_context)
{
	if (Instance == nullptr) {
		Instance = shared_from_this ();
	}

	m_pTimer = std::make_shared<boost::asio::deadline_timer> (_kIo_context);

	auto self (shared_from_this ());
	m_fnTick = [this, self](const boost::system::error_code& _kError_code)
	{
		if (_kError_code) {
			LOG_ERROR (_kError_code.message ());
		}
		else
		{
			tick ();

			if (m_pTimer != nullptr)
			{
				m_pTimer->expires_at (m_pTimer->expires_at () + m_kInterval);
				m_pTimer->async_wait (m_fnTick);
			}
		}
	};

	m_pTimer->expires_from_now (m_kInterval);
	m_pTimer->async_wait (m_fnTick);
}

void CEventManager::shutdown ()
{
	if (m_pTimer != nullptr)
	{
		m_pTimer->cancel ();
		m_pTimer = nullptr;
	}

	Instance = nullptr;
}

void CEventManager::add_event (const std::shared_ptr<CEvent>& _pEvent)
{
	long long time = _pEvent->get_time ();

	auto pos = m_kEvent_list.cbefore_begin ();
	for (auto it = m_kEvent_list.cbegin (); it != m_kEvent_list.cend (); it++)
	{
		pos = it;

		if (time > (*it)->get_time ())
		{
			break;
		}
	}

	m_kEvent_list.emplace_after (pos, _pEvent);
}

void CEventManager::tick ()
{
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now ().time_since_epoch ()).count ();
	while (!m_kEvent_list.empty ())
	{
		auto& pEvent = m_kEvent_list.front ();
		if (pEvent->is_valid ())
		{
			if (time >= pEvent->get_time ())
			{
				pEvent->excute ();
				m_kEvent_list.pop_front ();
			}
			else {
				break;
			}
		}
		else {
			m_kEvent_list.pop_front ();
		}
	}
}
