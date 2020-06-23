#pragma once

#include "network/CNetProtocol.h"

class ClientEcho : public CNetProtocol<ClientEcho>
{
public:
	ClientEcho ();
	ClientEcho (const std::string& _kString);
	virtual ~ClientEcho ();

	virtual void serialize (COutStream& _kOut_Stream) override;
	virtual void deserialize (CInStream& _kIn_Stream) override;
	virtual void excute () override;

private:
	std::string m_kString;
};