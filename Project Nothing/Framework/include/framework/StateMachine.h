#pragma once

template<typename TState> requires std::is_enum_v<TState>
class CStateMachine
{
	struct SState
	{
		SState (TState _nState, std::function<void ()>&& _rfnEnter, std::function<void ()>&& _rfnUpdate, std::function<void ()>&& _rfnLeave);
		virtual ~SState ();

		TState m_nState;
		std::function<void ()> m_fnEnter;
		std::function<void ()> m_fnUpdate;
		std::function<void ()> m_fnLeave;
	};

public:
	CStateMachine ();
	virtual ~CStateMachine ();

	void AddState (TState _nState, std::function<void ()>&& _rfnEnter, std::function<void ()>&& _rfnUpdate, std::function<void ()>&& _rfnLeave);
	void Update ();
	void Shutdown ();
	void SwitchTo (TState _nState);

private:
	std::shared_ptr<SState> m_pkCurrentState;
	std::map<TState, std::shared_ptr<SState>> m_kStateMap;
};

template<typename TState> requires std::is_enum_v<TState>
inline CStateMachine<TState>::SState::SState (TState _nState, std::function<void ()>&& _rfnEnter, std::function<void ()>&& _rfnUpdate, std::function<void ()>&& _rfnLeave)
	: m_nState (_nState)
	, m_fnEnter (std::move (_rfnEnter))
	, m_fnUpdate (std::move (_rfnUpdate))
	, m_fnLeave (std::move (_rfnLeave))
{
}

template<typename TState> requires std::is_enum_v<TState>
inline CStateMachine<TState>::SState::~SState ()
{
}

template<typename TState> requires std::is_enum_v<TState>
inline CStateMachine<TState>::CStateMachine ()
	: m_pkCurrentState (nullptr)
{
}

template<typename TState> requires std::is_enum_v<TState>
inline CStateMachine<TState>::~CStateMachine ()
{
}

template<typename TState> requires std::is_enum_v<TState>
inline void CStateMachine<TState>::AddState (TState _nState, std::function<void ()>&& _rfnEnter, std::function<void ()>&& _rfnUpdate, std::function<void ()>&& _rfnLeave)
{
	auto it = m_kStateMap.find (_nState);
	if (it == m_kStateMap.end ()) {
		m_kStateMap.emplace (_nState, std::make_shared<SState> (_nState,
			std::forward<std::function<void ()>> (_rfnEnter),
			std::forward<std::function<void ()>> (_rfnUpdate),
			std::forward<std::function<void ()>> (_rfnLeave)));
	}
}

template<typename TState> requires std::is_enum_v<TState>
inline void CStateMachine<TState>::Update ()
{
	if (m_pkCurrentState != nullptr && m_pkCurrentState->m_fnUpdate != nullptr) {
		m_pkCurrentState->m_fnUpdate ();
	}
}

template<typename TState> requires std::is_enum_v<TState>
inline void CStateMachine<TState>::Shutdown ()
{
	if (m_pkCurrentState != nullptr && m_pkCurrentState->m_fnUpdate != nullptr) {
		m_pkCurrentState->m_fnLeave ();
	}

	m_pkCurrentState = nullptr;
}

template<typename TState> requires std::is_enum_v<TState>
inline void CStateMachine<TState>::SwitchTo (TState _nState)
{
	auto it = m_kStateMap.find (_nState);
	if (it == m_kStateMap.end ()) {
		return;
	}


	if (m_pkCurrentState != nullptr && m_pkCurrentState->m_fnUpdate != nullptr) {
		m_pkCurrentState->m_fnLeave ();
	}

	std::shared_ptr<SState> newState = it->second;
	if (newState != nullptr && newState->m_fnEnter != nullptr) {
		newState->m_fnEnter ();
	}

	m_pkCurrentState = newState;
}
