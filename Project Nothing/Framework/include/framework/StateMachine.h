#pragma once

template<typename T> requires std::is_enum_v<T>
class CStateMachine
{
public:
	CStateMachine ();
	virtual ~CStateMachine ();
};

template<typename T> requires std::is_enum_v<T>
inline CStateMachine<T>::CStateMachine ()
{
}

template<typename T> requires std::is_enum_v<T>
inline CStateMachine<T>::~CStateMachine ()
{
}
