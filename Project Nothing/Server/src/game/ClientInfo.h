#pragma once

#include "game/UserCommand.h"

class CClientInfo
{
public:
	CClientInfo ();
	virtual ~CClientInfo ();

public:
	uint32_t m_nPlayerID;
	SUserCommand m_kLatestCommand;
};
