#pragma once

struct SUserCommand : public IBitSerializable
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
		SButtonBitField (EButton _nButton);
		virtual ~SButtonBitField ();

		inline bool IsSet (EButton _nButton) { return (static_cast<uint32_t> (m_nFlags) & static_cast<uint32_t> (_nButton)) > 0; }

	public:
		EButton m_nFlags;
	};

public:
	SUserCommand ();
	SUserCommand (uint32_t _nTick);
	virtual ~SUserCommand ();

	void Reset ();

	virtual void Serialize (CBitOutStream& _rkOutStream) override;
	virtual void Deserialize (CBitInStream& _rkInStream) override;

public:
	uint32_t m_nTick;
	SButtonBitField m_kButtons;
};