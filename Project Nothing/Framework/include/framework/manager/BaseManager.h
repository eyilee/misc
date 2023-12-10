#pragma once

class IBaseManager
{
public:
	IBaseManager ();
	virtual ~IBaseManager ();
};

template<typename T>
class CBaseManager : public IBaseManager, public std::enable_shared_from_this<T>
{
public:
	CBaseManager ();
	virtual ~CBaseManager ();

public:
	static std::shared_ptr<T> Instance;
};

template<typename T>
inline CBaseManager<T>::CBaseManager ()
{
}

template<typename T>
inline CBaseManager<T>::~CBaseManager ()
{
}

template<typename T>
std::shared_ptr<T> CBaseManager<T>::Instance = nullptr;
