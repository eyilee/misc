#include "stdafx.h"
#include "game/UserCommand.h"

SUserCommand::SButtonBitField::SButtonBitField (EButton _nButton)
	: m_nFlags (_nButton)
{
}

SUserCommand::SButtonBitField::~SButtonBitField ()
{
}

SUserCommand::SUserCommand ()
	: m_nTick (0)
	, m_kButtons (EButton::None)
{
}

SUserCommand::SUserCommand (uint32_t _nTick)
	: m_nTick (_nTick)
	, m_kButtons (EButton::None)
{
}

SUserCommand::~SUserCommand ()
{
}

void SUserCommand::Reset ()
{
	m_nTick = 0;
	m_kButtons.m_nFlags = EButton::None;
}

void SUserCommand::Serialize (CBitOutStream& _rkOutStream)
{
	_rkOutStream.Write (m_nTick);
	_rkOutStream.Write (static_cast<uint32_t> (m_kButtons.m_nFlags));
}

void SUserCommand::Deserialize (CBitInStream& _rkInStream)
{
	_rkInStream.Read (m_nTick);

	uint32_t flags;
	_rkInStream.Read (flags);
	m_kButtons.m_nFlags = static_cast<EButton> (flags);
}
