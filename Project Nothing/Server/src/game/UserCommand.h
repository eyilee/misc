#pragma once

struct SUserCommand
{
public:
	enum class EButton : uint32_t
	{
		None = 0,
		Up = 1 << 0,
		Down = 1 << 1,
		Left = 1 << 2,
		Right = 1 << 3,
		Accel = 1 << 4,
		Melee = 1 << 5,
		Shoot = 1 << 6,
	};

	struct SButtonBitField
	{
	public:
		EButton m_nFlags;

		bool IsSet (EButton _nButton)
		{
			return (static_cast<uint32_t> (m_nFlags) & static_cast<uint32_t> (_nButton)) > 0;
		}
	};

	uint32_t m_nTick;
	SButtonBitField m_kButtons;

	SUserCommand ()
	{
		Reset ();
	}

	SUserCommand (uint32_t _nTick)
	{
		m_nTick = _nTick;
		m_kButtons.m_nFlags = EButton::None;
	}

	void Reset ()
	{
		m_nTick = 0;
		m_kButtons.m_nFlags = EButton::None;
	}
};