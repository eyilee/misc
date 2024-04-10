using UnityEngine;

namespace ProjectNothing
{
    public sealed class GameManager
    {
        static Game m_Game = null;
        static GameObject m_PlayerPrefab = null;

        static bool m_IsInit = false;

        public static void Init (GameObject playerPrefab)
        {
            m_PlayerPrefab = playerPrefab;
            m_IsInit = true;
        }

        public static void Shutdown ()
        { 
        }

        public static void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            if (m_Game != null)
            {
                ulong duration = (ulong)(Time.deltaTime * 1000);
                m_Game.Update (duration);
            }
        }

        public static void CreateGame (uint gameID, ClientConnection connection)
        {
            if (m_Game != null)
            {
                return;
            }

            m_Game = new Game (gameID, connection);
        }

        public static void JoinGame (uint gameID, ushort port, uint key)
        {
            if (m_Game == null || m_Game.GameID != gameID)
            {
                return;
            }

            NetworkManager.UdpConnect (m_Game.Connection, port, key);

            m_Game.Connection.Init ();
            m_Game.Load (m_PlayerPrefab);
            m_Game.Start ();
        }

        public static void LeaveGame ()
        {
            m_Game = null;
        }
    }
}