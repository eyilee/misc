#include "stdafx.h"
#include "framework/event/Event.h"

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
