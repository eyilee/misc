#include "stdafx.h"
#include "logger/Logger.h"
#include "framework/GameLoop.h"
#include "framework/Time.h"
#include "framework/manager/GameManager.h"

CGameManager::CGameManager ()
	: m_bIsRunning (false)
	, m_kInterval (boost::posix_time::milliseconds (10))
	, m_pkTimer (nullptr)
{
}

CGameManager::~CGameManager ()
{
}

void CGameManager::Init (boost::asio::io_context& _rkContext)
{
	if (Instance != nullptr) {
		return;
	}

	Instance = std::make_shared<CGameManager> ();
	Instance->Run (_rkContext);
}

void CGameManager::Shutdown ()
{
	if (Instance == nullptr) {
		return;
	}

	Instance->Stop ();
	Instance = nullptr;
}

void CGameManager::Run (boost::asio::io_context& _rkContext)
{
	m_bIsRunning = true;

	auto self (shared_from_this ());
	m_fnTick = [this, self](const boost::system::error_code& _rkErrorCode)
		{
			if (!m_bIsRunning)
			{
				for (auto& pair : m_kGameLoops) {
					pair.second->Shutdown ();
				}

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
					Update ();

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

void CGameManager::Stop ()
{
	m_bIsRunning = false;
}

void CGameManager::Update ()
{
	CTime::FrameTime = CTime::GetMiliSecond ();

	for (auto& pair : m_kGameLoops) {
		pair.second->Update ();
	}
}
