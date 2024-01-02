using System;

public struct UserCommand
{
    [Flags]
    public enum Button : uint
    {
        None = 0,
        Forward = 1 << 0,
        Backward = 1 << 1,
        TurnLeft = 1 << 2,
        TurnRight = 1 << 3,
        Accel = 1 << 4,
        Melee = 1 << 5,
        Shoot = 1 << 6,
    }

    public struct ButtonBitField
    {
        public Button flags;

        public readonly bool IsSet (Button button)
        {
            return (flags & button) > 0;
        }

        public void Or (Button button, bool isOn)
        {
            if (isOn)
            {
                flags |= button;
            }
        }

        public void Set (Button button, bool isOn)
        {
            if (isOn)
            {
                flags |= button;
            }
            else
            {
                flags &= ~button;
            }
        }
    }

    public int tick;
    public ButtonBitField buttons;
}
