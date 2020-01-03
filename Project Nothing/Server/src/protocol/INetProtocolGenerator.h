#pragma once

class INetProtocol;

class INetProtocolGenerator
{
public:
	INetProtocolGenerator ();
	virtual ~INetProtocolGenerator ();

	virtual std::shared_ptr<INetProtocol> generate () = 0;
};
