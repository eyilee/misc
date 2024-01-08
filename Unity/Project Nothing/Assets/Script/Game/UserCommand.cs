using System;

namespace ProjectNothing
{
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
            public Button m_Flags;

            public readonly bool IsSet (Button button)
            {
                return (m_Flags & button) > 0;
            }

            public void Or (Button button, bool isOn)
            {
                if (isOn)
                {
                    m_Flags |= button;
                }
            }

            public void Set (Button button, bool isOn)
            {
                if (isOn)
                {
                    m_Flags |= button;
                }
                else
                {
                    m_Flags &= ~button;
                }
            }
        }

        public uint m_Tick;
        public ButtonBitField m_Buttons;

        public static readonly UserCommand m_DefaultCommand = new (0);

        public UserCommand (uint tick)
        {
            m_Tick = tick;
            m_Buttons.m_Flags = Button.None;
        }
    }
}
