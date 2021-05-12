#include "stdafx.h"

#include "framework/event/CEvent.h"

IEvent::IEvent ()
{
}

IEvent::~IEvent ()
{
}

CEvent::CEvent ()
	: m_nTime (0)
	, m_bIs_valid (true)
{
}

CEvent::~CEvent ()
{
}
