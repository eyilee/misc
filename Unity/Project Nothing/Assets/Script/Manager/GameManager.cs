using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class GameManager : MonoSingleton<GameManager>
    {
        [SerializeField]
        GameConfig m_Config;

        [SerializeField]
        public GameObject m_PlayerPrefab;

        private Game m_Game = null;

        private bool m_IsInit = false;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public IEnumerator Start ()
        {
            Initialize (gameObject);

            ProtocolManager.Init ();

            yield return NetworkManager.Init (m_Config.Host, m_Config.Port);

            m_IsInit = true;
        }

        public void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            NetworkManager.Update ();

            m_Game?.Update ();
        }

        public void CreateGame ()
        {
            m_Game = new Game ();
            m_Game.Load (m_PlayerPrefab);
            m_Game.Start ();
        }
    }
}