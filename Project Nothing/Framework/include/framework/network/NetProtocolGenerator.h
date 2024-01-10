#pragma once

class INetProtocol;

class INetProtocolGenerator
{
public:
	INetProtocolGenerator ();
	virtual ~INetProtocolGenerator ();

	virtual std::shared_ptr<INetProtocol> Generate () = 0;
};

template<typename T> requires std::is_base_of_v<INetProtocol, T>
class CNetProtocolGenerator : public INetProtocolGenerator
{
public:
	CNetProtocolGenerator ();
	virtual ~CNetProtocolGenerator ();

	virtual std::shared_ptr<INetProtocol> Generate ();
};

template<typename T> requires std::is_base_of_v<INetProtocol, T>
inline CNetProtocolGenerator<T>::CNetProtocolGenerator ()
{
}

template<typename T> requires std::is_base_of_v<INetProtocol, T>
inline CNetProtocolGenerator<T>::~CNetProtocolGenerator ()
{
}

template<typename T> requires std::is_base_of_v<INetProtocol, T>
inline std::shared_ptr<INetProtocol> CNetProtocolGenerator<T>::Generate ()
{
	return std::make_shared<T> ();
}
