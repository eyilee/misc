using UnityEngine;

namespace ProjectNothing
{
    public class Game
    {
        private GameObject m_Player = null;
        private PlayerController m_PlayerController = null;

        private bool m_IsRunning;

        public void Load (GameObject playerPrefab)
        {
            m_Player = Object.Instantiate (playerPrefab);

            m_PlayerController = new PlayerController ();
            m_PlayerController.Init ();
        }

        public void Start ()
        {
            m_IsRunning = true;
        }

        public void Update ()
        {
            if (!m_IsRunning)
            {
                return;
            }

            m_PlayerController.Update ();
        }
    }
}
