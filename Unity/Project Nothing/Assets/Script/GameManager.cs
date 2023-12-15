using ProjectNothing.Network;
using ProjectNothing.Protocol;
using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class GameManager : MonoSingleton<GameManager>
    {
        public int m_ID = 1001;
        public uint m_Key = 0;
        public bool m_Login = false;

        private bool m_IsInit = false;
        private float m_Time = 0.0f;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public IEnumerator Start ()
        {
            Initialize (gameObject);

            NetworkManager.Initialize (gameObject);
            yield return NetworkManager.Init ("127.0.0.1", 8484, 8485);

            ProtocolManager.Initialize (gameObject);
            ProtocolManager.Instance.Init ();

            NetworkManager.ComposeTcpOutput (new ServerLogin { m_ID = m_ID });

            m_IsInit = true;
        }

        public void Update ()
        {
            if (m_IsInit)
            {
                m_Time += Time.deltaTime;

                if (m_Login)
                {
                    if (m_Time >= 1f)
                    {
                        NetworkManager.ComposeTcpOutput (new ServerEcho { m_String = "你好！" });
                        m_Time = 0.0f;
                    }
                }
            }
        }
    }
}