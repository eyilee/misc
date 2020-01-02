#pragma once

class INetProtocol;

template <typename T>
class INetProtocolGenerator
{
	INetProtocolGenerator ();
	virtual ~INetProtocolGenerator ();

	virtual std::shared_ptr<INetProtocol> generate ();
};

template<typename T>
inline INetProtocolGenerator<T>::~INetProtocolGenerator ()
{
}

template<typename T>
inline std::shared_ptr<INetProtocol> INetProtocolGenerator<T>::generate ()
{
	return std::make_shared<T> ();
}
