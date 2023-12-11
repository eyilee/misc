using ProjectNothing.Network;
using ProjectNothing.Protocol;
using System;
using System.Collections;
using System.Text;
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

                if (m_Time >= 1f)
                {
                    NetworkManager.Instance.m_NetBridge.ComposeOutput (new ServerEcho { m_String = "你好！" });
                    m_Time = 0.0f;
                }
            }
        }

        public IEnumerator Start ()
        {
            NetworkManager.Initialize (gameObject);
            yield return NetworkManager.Instance.Init ("127.0.0.1", 8484);

            ProtocolManager.Initialize (gameObject);
            ProtocolManager.Instance.Init ();

            m_IsInit = true;
        }
    }
}