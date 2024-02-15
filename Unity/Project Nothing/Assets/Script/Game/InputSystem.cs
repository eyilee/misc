using System;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class InputSystem
    {
        [Flags]
        public enum EBlockType
        {
            None = 0,
            Console = 1,
            Chat = 2,
            Debug = 4,
        }

        public void SetBlock (EBlockType blockType, bool isBlock)
        {
            if (isBlock)
            {
                m_BlockType |= blockType;
            }
            else
            {
                m_BlockType &= ~blockType;
            }
        }

        EBlockType m_BlockType = EBlockType.None;

        public float GetAxisRaw (string axis)
        {
            return m_BlockType != EBlockType.None ? 0.0f : Input.GetAxisRaw (axis);
        }

        public bool GetKey (KeyCode key)
        {
            return m_BlockType == EBlockType.None && Input.GetKey (key);
        }

        public bool GetKeyDown (KeyCode key)
        {
            return m_BlockType == EBlockType.None && Input.GetKeyDown (key);
        }

        public bool GetKeyUp (KeyCode key)
        {
            return m_BlockType == EBlockType.None && Input.GetKeyUp (key);
        }

        public bool GetMouseButton (int button)
        {
            return m_BlockType == EBlockType.None && Input.GetMouseButton (button);
        }

        public void SampleInput (ref UserCommand userCommand)
        {
            userCommand.m_Buttons.Or (UserCommand.EButton.Up, GetKey (KeyCode.UpArrow));
            userCommand.m_Buttons.Or (UserCommand.EButton.Down, GetKey (KeyCode.DownArrow));
            userCommand.m_Buttons.Or (UserCommand.EButton.Left, GetKey (KeyCode.LeftArrow));
            userCommand.m_Buttons.Or (UserCommand.EButton.Right, GetKey (KeyCode.RightArrow));
            userCommand.m_Buttons.Or (UserCommand.EButton.Accel, GetKey (KeyCode.A));
            userCommand.m_Buttons.Or (UserCommand.EButton.Melee, GetKeyDown (KeyCode.S));
            userCommand.m_Buttons.Or (UserCommand.EButton.Shoot, GetKeyDown (KeyCode.D));
        }

        public void ClearInput (ref UserCommand userCommand)
        {
            userCommand.m_Buttons.m_Flags = UserCommand.EButton.None;
        }
    }
}
