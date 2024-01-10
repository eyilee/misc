#pragma once
#include "framework/Random.h"
#include "framework/manager/BaseManager.h"

class IGameLoop;

class CGameManager : public CBaseManager<CGameManager>
{
public:
	CGameManager ();
	virtual ~CGameManager ();

	static void Init (boost::asio::io_context& _rkContext);
	static void Shutdown ();

	template<typename T> requires std::is_base_of_v<IGameLoop, T>
	static std::shared_ptr<T> CreateGame ();

	template<typename T = IGameLoop> requires std::is_base_of_v<IGameLoop, T>
	static std::shared_ptr<T> GetGame (uint32_t _nID);

private:
	void Run (boost::asio::io_context& _rkContext);
	void Stop ();
	void Update ();

	template<typename T> requires std::is_base_of_v<IGameLoop, T>
	std::shared_ptr<T> Create ();

	template<typename T = IGameLoop> requires std::is_base_of_v<IGameLoop, T>
	std::shared_ptr<T> Get (uint32_t _nID);

private:
	std::function<void (const boost::system::error_code& _rkErrorCode)> m_fnTick;

private:
	bool m_bIsRunning;
	boost::posix_time::milliseconds m_kInterval;
	std::shared_ptr<boost::asio::deadline_timer> m_pkTimer;

	std::map<uint32_t, std::shared_ptr<IGameLoop>> m_kGameLoops;
};

template<typename T> requires std::is_base_of_v<IGameLoop, T>
inline std::shared_ptr<T> CGameManager::CreateGame ()
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->Create<T> ();
}

template<typename T> requires std::is_base_of_v<IGameLoop, T>
inline std::shared_ptr<T> CGameManager::GetGame (uint32_t _nID)
{
	if (Instance == nullptr) {
		return nullptr;
	}

	return Instance->Get<T> (_nID);
}

template<typename T> requires std::is_base_of_v<IGameLoop, T>
inline std::shared_ptr<T> CGameManager::Create ()
{
	uint32_t id = 0;
	while (id == 0 || m_kGameLoops.find (id) != m_kGameLoops.end ()) {
		id = CRandom::GetValue<uint32_t> ();
	}

	std::shared_ptr<T> gameLoop = std::make_shared<T> ();
	gameLoop->SetID (id);
	m_kGameLoops.emplace (id, gameLoop);
	return gameLoop;
}

template<typename T> requires std::is_base_of_v<IGameLoop, T>
inline std::shared_ptr<T> CGameManager::Get (uint32_t _nID)
{
	auto it = m_kGameLoops.find (_nID);
	if (it != m_kGameLoops.end ()) {
		return std::static_pointer_cast<T> (it->second);
	}

	return nullptr;
}
