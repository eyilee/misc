using System.Collections;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace ProjectNothing
{
    public class MainUI : MonoBehaviour
    {
        private Button m_ConnectButton;
        private TextMeshProUGUI m_ConnectText;

        private Button m_CreateButton;
        private TextMeshProUGUI m_CreateText;

        private Button m_ShutdownButton;
        private TextMeshProUGUI m_ShutdownText;

        private TextMeshProUGUI m_LatencyText;

        void Awake ()
        {
            m_ConnectButton = transform.Find ("buttons/btn_connect").GetComponent<Button> ();
            m_ConnectButton.onClick.AddListener (Connect);

            m_ConnectText = transform.Find ("buttons/btn_connect/txt_connect").GetComponent<TextMeshProUGUI> ();
            m_ConnectText.text = "連線";

            m_CreateButton = transform.Find ("buttons/btn_create").GetComponent<Button> ();
            m_CreateButton.onClick.AddListener (CreateGame);

            m_CreateText = transform.Find ("buttons/btn_create/txt_create").GetComponent<TextMeshProUGUI> ();
            m_CreateText.text = "創建遊戲";

            m_ShutdownButton = transform.Find ("buttons/btn_shutdown").GetComponent<Button> ();
            m_ShutdownButton.onClick.AddListener (ShutdownServer);

            m_ShutdownText = transform.Find ("buttons/btn_shutdown/txt_shutdown").GetComponent<TextMeshProUGUI> ();
            m_ShutdownText.text = "關閉伺服器";

            m_LatencyText = transform.Find ("txt_latency").GetComponent<TextMeshProUGUI> ();
            StartCoroutine (RefreshLatency ());
        }

        IEnumerator RefreshLatency ()
        {
            WaitForSeconds wait = new (1f);

            while (true)
            {
                m_LatencyText.text = string.Format ("ping: {0} ms", NetworkManager.m_Latency);
                yield return wait;
            }
        }

        void Connect ()
        {
            NetworkManager.ComposeTcpOutput (new NC_ServerLogin { m_ID = 1 });
        }

        void CreateGame ()
        {
            NetworkManager.ComposeTcpOutput (new NC_ServerCreateGame ());
        }

        void ShutdownServer ()
        {
            NetworkManager.ComposeTcpOutput (new NC_ServerShutdown ());
        }
    }
}
