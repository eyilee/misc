#pragma once

class INetProtocolGenerator;

template <typename T>
class CNetProtocolGenerator : public INetProtocolGenerator
{
public:
	CNetProtocolGenerator ();
	virtual ~CNetProtocolGenerator ();

	virtual std::shared_ptr<INetProtocol> generate ();
};

template<typename T>
inline CNetProtocolGenerator<T>::CNetProtocolGenerator ()
{
}

template<typename T>
inline CNetProtocolGenerator<T>::~CNetProtocolGenerator ()
{
}

template<typename T>
inline std::shared_ptr<INetProtocol> CNetProtocolGenerator<T>::generate ()
{
	return std::make_shared<T> ();
}
