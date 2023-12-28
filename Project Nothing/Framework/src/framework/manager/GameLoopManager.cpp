#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/Time.h"
#include "framework/manager/GameLoopManager.h"

CGameLoopManager::CGameLoopManager ()
	: m_bIsRunning (false)
	, m_kInterval (boost::posix_time::milliseconds (1))
	, m_pkTimer (nullptr)
{
}

CGameLoopManager::~CGameLoopManager ()
{
}

void CGameLoopManager::Init (boost::asio::io_context& _rkContext)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CGameLoopManager> ();
	Instance->Run (_rkContext);
}

void CGameLoopManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Stop ();
	Instance = nullptr;
}

void CGameLoopManager::Run (boost::asio::io_context& _rkContext)
{
	m_bIsRunning = true;

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
						m_pkTimer->expires_from_now (m_kInterval);
						m_pkTimer->async_wait (m_fnTick);
					}
				}
			}
		};

	m_pkTimer = std::make_shared<boost::asio::deadline_timer> (_rkContext);
	m_pkTimer->expires_from_now (m_kInterval);
	m_pkTimer->async_wait (m_fnTick);
}

void CGameLoopManager::Stop ()
{
	m_bIsRunning = false;
}

void CGameLoopManager::Tick ()
{
}
