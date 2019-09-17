#include "stdafx.h"

#include "CTcpConnection.h"
#include "CTcpSession.h"

CTcpSession::CTcpSession (const std::shared_ptr<CTcpConnection>& _kConnection)
	: m_kConnection (_kConnection->shared_from_this ())
{
}

CTcpSession::~CTcpSession ()
{
}
