#include "stdafx.h"

#include "framework/manager/CBaseManager.h"
#include "framework/manager/CEventManager.h"

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
		if (_kError_code && _kError_code != boost::asio::error::operation_aborted) {
			LOG_ERROR (_kError_code.message ());
		}

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now ().time_since_epoch ());
		LOG ("Tick: current time %lld.", time.count ());

		if (m_pTimer != nullptr)
		{
			m_pTimer->expires_at (m_pTimer->expires_at () + m_kInterval);
			m_pTimer->async_wait (m_fnTick);
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
