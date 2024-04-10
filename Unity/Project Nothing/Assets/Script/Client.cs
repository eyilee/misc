using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class Client : MonoBehaviour
    {
        [SerializeField]
        GameConfig m_Config;

        [SerializeField]
        GameObject m_PlayerPrefab;

        bool m_IsInit = false;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public IEnumerator Start ()
        {
            InitProtocolManager ();
            yield return InitNetworkManager ();
            InitGameManager ();

            m_IsInit = true;
        }

        public void OnDestroy ()
        {
            if (!m_IsInit)
            {
                return;
            }

            NetworkManager.Shutdown ();
            GameManager.Shutdown ();
        }

        public void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            NetworkManager.Update ();
            GameManager.Update ();
        }

        void InitProtocolManager ()
        {
            ProtocolManager.Init ();

            ProtocolManager.RegisterNetCommand<NC_ServerLogin> (1);
            ProtocolManager.RegisterNetEvent<NE_ClientLoginResult> (51);

            ProtocolManager.RegisterNetCommand<NC_ServerEcho> (100);
            ProtocolManager.RegisterNetCommand<NC_ServerCreateGame> (101);
            ProtocolManager.RegisterNetCommand<NC_ServerJoinGame> (102);
            ProtocolManager.RegisterNetEvent<NE_ClientEchoResult> (200);
            ProtocolManager.RegisterNetEvent<NE_ClientCreateGameResult> (201);
            ProtocolManager.RegisterNetEvent<NE_ClientJoinGameResult> (202);

            ProtocolManager.RegisterNetCommand<NC_ServerShutdown> (9000);
        }

        public IEnumerator InitNetworkManager ()
        {
            yield return NetworkManager.Init (m_Config.Host, m_Config.Port);
        }

        void InitGameManager ()
        {
            GameManager.Init (m_PlayerPrefab);
        }
    }
}
