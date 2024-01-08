#include "stdafx.h"
#include "framework/Event.h"

IEvent::IEvent ()
	: m_nTime (0)
	, m_bIsValid (true)
{
}

IEvent::~IEvent ()
{
}
