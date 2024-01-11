using UnityEngine;

namespace ProjectNothing
{
    public class PlayerController
    {
        public const uint COMMAND_BUFFER_SIZE = 32;

        InputSystem m_InputSystem = null;
        UserCommand m_UserCommand = new (0);
        CommandBuffer<UserCommand> m_CommandBuffer = new (COMMAND_BUFFER_SIZE);

        GameObject m_Player = null;

        public void Init ()
        {
            m_InputSystem = new InputSystem ();
        }

        public void SetPlayer (GameObject player)
        {
            m_Player = player;
        }

        public void Update ()
        {
        }

        public void SampleInput (uint tick)
        {
            m_InputSystem.SampleInput (ref m_UserCommand);

            m_UserCommand.m_Tick = tick;
        }

        public void ClearInput ()
        {
            m_InputSystem.ClearInput (ref m_UserCommand);
        }

        public void StoreCommand (uint tick)
        {
            uint lastTick = 0;
            if (m_CommandBuffer.TryGetLastTick (ref lastTick))
            {
                if (tick != lastTick && tick != lastTick + 1)
                {
                    m_CommandBuffer.Clear ();
                }
            }

            if (tick == lastTick)
            {
                m_CommandBuffer.Set (tick, ref m_UserCommand);
            }
            else
            {
                m_CommandBuffer.Add (tick, ref m_UserCommand);
            }
        }

        public void RetrieveCommand (uint tick)
        {
            UserCommand command = UserCommand.m_DefaultCommand;
            if (m_CommandBuffer.TryGetValue (tick, ref command))
            {
                if (command.m_Buttons.IsSet (UserCommand.Button.Up))
                {
                    m_Player.transform.Translate (Vector3.up * 0.05f);
                }

                if (command.m_Buttons.IsSet (UserCommand.Button.Down))
                {
                    m_Player.transform.Translate (Vector3.down * 0.05f);
                }
            }
        }

        public void SendCommand (uint tick)
        {

        }

        public bool HasCommands (uint firstCommandTick, uint lastCommandTick)
        {
            uint firstTick = 0;
            uint lastTick = 0;
            if (m_CommandBuffer.TryGetFirstTick (ref firstTick) && m_CommandBuffer.TryGetLastTick (ref lastTick))
            {
                return firstCommandTick >= firstTick && lastCommandTick <= lastTick;
            }

            return false;
        }
    }
}
