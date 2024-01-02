using System;
using UnityEngine;

public class InputSystem
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
        userCommand.buttons.Or (UserCommand.Button.Forward, GetKey (KeyCode.UpArrow));
        userCommand.buttons.Or (UserCommand.Button.Backward, GetKey (KeyCode.DownArrow));
        userCommand.buttons.Or (UserCommand.Button.TurnLeft, GetKey (KeyCode.LeftArrow));
        userCommand.buttons.Or (UserCommand.Button.TurnRight, GetKey (KeyCode.RightArrow));
        userCommand.buttons.Or (UserCommand.Button.Accel, GetKey (KeyCode.A));
        userCommand.buttons.Or (UserCommand.Button.Melee, GetKeyDown (KeyCode.S));
        userCommand.buttons.Or (UserCommand.Button.Shoot, GetKeyDown (KeyCode.D));
    }

    public void ClearInput ()
    {
    }
}


