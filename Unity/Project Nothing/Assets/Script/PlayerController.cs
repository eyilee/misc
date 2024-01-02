using UnityEngine;

namespace ProjectNothing
{
    public class PlayerController
    {
        private InputSystem m_InputSystem = null;
        private UserCommand m_UserCommand = new ();

        public void Init ()
        {
            m_InputSystem = new InputSystem ();
        }

        public void Update ()
        {
            m_InputSystem.SampleInput (ref m_UserCommand);

            Debug.Log ((uint)m_UserCommand.buttons.flags);
        }
    }
}
