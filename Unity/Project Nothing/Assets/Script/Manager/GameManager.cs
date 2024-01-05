using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class GameManager : MonoSingleton<GameManager>
    {
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

            yield return NetworkManager.Init ("127.0.0.1", 8484);

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