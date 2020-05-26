using ProjectNothing.Network;
using ProjectNothing.Protocol;
using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class GameManager : MonoBehaviour
    {
        private bool m_IsInit = false;
        private float m_Time = 0.0f;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public void Update ()
        {
            if (m_IsInit)
            {
                m_Time += Time.deltaTime;

                if (m_Time >= 0.025f)
                {
                    NetworkManager.Instance.m_NetBridge.ComposeOutput (new ServerEcho { m_String = "Hello World!" });
                    m_Time = 0.0f;
                }
            }
        }

        public IEnumerator Start ()
        {
            yield return NetworkManager.Initialize (gameObject);

            if (NetworkManager.Instance != null)
            {
                NetworkManager.Instance.Init ("127.0.0.1", 8484);
                Debug.Log ("NetworkManager exist.");
            }

            yield return ProtocolManager.Initialize (gameObject);

            if (ProtocolManager.Instance != null)
            {
                ProtocolManager.Instance.Init ();
                Debug.Log ("ProtocolManager exist.");
            }

            yield return SelectionManager.Initialize (gameObject);

            if (SelectionManager.Instance != null)
            {
                Debug.Log ("SelectionManager exist.");
            }

            m_IsInit = true;
        }
    }
}