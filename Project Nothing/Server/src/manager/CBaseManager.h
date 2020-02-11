#pragma once

class IBaseManager
{
public:
	IBaseManager ();
	virtual ~IBaseManager ();
};

template <typename T>
class CBaseManager : public IBaseManager, public std::enable_shared_from_this<T>
{
public:
	CBaseManager ();
	virtual ~CBaseManager ();
};

template<typename T>
inline CBaseManager<T>::CBaseManager ()
{
}

template<typename T>
inline CBaseManager<T>::~CBaseManager ()
{
}
