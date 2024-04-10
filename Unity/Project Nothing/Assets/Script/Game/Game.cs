using System;
using UnityEngine;

namespace ProjectNothing
{
    public class Game
    {
        readonly uint m_GameID;
        readonly ClientConnection m_Connection;

        public uint GameID => m_GameID;
        public ClientConnection Connection => m_Connection;

        private GameObject m_Player = null;
        private PlayerController m_PlayerController = null;

        GameTime m_PredictTime;
        GameTime m_RenderTime;
        GameTime m_ServerTime;

        private bool m_IsRunning;

        public Game (uint gameID, ClientConnection connection)
        {
            m_GameID = gameID;
            m_Connection = connection;
        }

        public void Load (GameObject playerPrefab)
        {
            m_Player = UnityEngine.Object.Instantiate (playerPrefab);

            m_PlayerController = new PlayerController (this);
            m_PlayerController.Init ();
            m_PlayerController.SetPlayer (m_Player);
        }

        public void Start ()
        {
            m_PredictTime = new GameTime (60);
            m_RenderTime = new GameTime (60);
            m_ServerTime = new GameTime (60);

            m_IsRunning = true;
        }

        public void Update (ulong duration)
        {
            if (!m_IsRunning)
            {
                return;
            }

            UpdateTime (duration);

            // TODO: update state

            if (IsPredictionAllowed ())
            {

            }

            // simulate server result
            // uint prevServerTick = m_ServerTime.m_Tick;
            // m_ServerTime.AddDuration (duration);
            // 
            // for (uint tick = prevServerTick; tick < m_ServerTime.m_Tick; tick++)
            // {
            //     m_PlayerController.RetrieveCommand (tick);
            // }

            m_Connection.ComposePackage ();
        }

        void UpdateTime (ulong duration)
        {
            uint prevTick = m_PredictTime.m_Tick;
            m_PlayerController.SampleInput (prevTick);

            m_PredictTime.AddDuration (duration);

            if (m_PredictTime.m_Tick > prevTick)
            {
                uint oldestCommandTick = Math.Max (prevTick, m_PredictTime.m_Tick > PlayerController.COMMAND_BUFFER_SIZE ? m_PredictTime.m_Tick - PlayerController.COMMAND_BUFFER_SIZE : 0);
                for (uint tick = oldestCommandTick; tick < m_PredictTime.m_Tick; tick++)
                {
                    m_PlayerController.StoreCommand (tick);
                    m_PlayerController.SendCommand (tick);
                }

                m_PlayerController.ClearInput ();
                m_PlayerController.SampleInput (m_PredictTime.m_Tick);
                m_PlayerController.StoreCommand (m_PredictTime.m_Tick);
            }

            m_PlayerController.StoreCommand (m_PredictTime.m_Tick);
        }

        bool IsPredictionAllowed ()
        {
            if (m_PredictTime.m_Tick <= m_ServerTime.m_Tick)
            {
                return false;
            }

            if (!m_PlayerController.HasCommands (m_ServerTime.m_Tick + 1, m_PredictTime.m_Tick))
            {
                return false;
            }

            return true;
        }
    }
}
