#pragma once

#include "network/CNetProtocol.h"

class ServerEcho : public CNetProtocol<ServerEcho>
{
public:
	ServerEcho ();
	virtual ~ServerEcho ();

	virtual void serialize (COutStream& _kOut_Stream) override;
	virtual void deserialize (CInStream& _kIn_Stream) override;
	virtual void excute () override;

private:
	std::string m_kString;
};