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

        Game m_Game = null;

        uint m_TickStep = 1;
        ulong m_TickInterval = 16;

        bool m_IsInit = false;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public IEnumerator Start ()
        {
            Initialize (gameObject);

            ProtocolManager.Init ();

            yield return NetworkManager.Init (m_Config.Host, m_Config.Port);

            m_TickStep = 1;
            m_TickInterval = (ulong)(1000 / m_Config.TickRate);

            m_IsInit = true;
        }

        public void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            NetworkManager.Update ();

            ulong duration = (ulong)(Time.deltaTime * 1000);
            m_Game?.Update (duration);
        }

        public void CreateGame ()
        {
            m_Game = new Game ();
            m_Game.Load (m_PlayerPrefab);
            m_Game.Start ();
        }
    }
}