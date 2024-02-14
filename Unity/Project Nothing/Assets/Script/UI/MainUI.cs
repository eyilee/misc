using System.Collections;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

namespace ProjectNothing
{
    public class MainUI : MonoBehaviour
    {
        private Button m_ConnectButton;
        private Button m_CreateButton;
        private Button m_ShutdownButton;

        private TextMeshProUGUI m_LatencyText;

        void Awake ()
        {
            m_ConnectButton = transform.Find ("buttons/btn_connect").GetComponent<Button> ();
            m_ConnectButton.onClick.AddListener (Connect);

            m_CreateButton = transform.Find ("buttons/btn_create").GetComponent<Button> ();
            m_CreateButton.onClick.AddListener (CreateGame);

            m_ShutdownButton = transform.Find ("buttons/btn_shutdown").GetComponent<Button> ();
            m_ShutdownButton.onClick.AddListener (ShutdownServer);

            m_LatencyText = transform.Find ("txt_latency").GetComponent<TextMeshProUGUI> ();
            StartCoroutine (RefreshLatency ());
        }

        IEnumerator RefreshLatency ()
        {
            WaitForSeconds wait = new (1f);

            while (true)
            {
                //m_LatencyText.text = string.Format ("ping: {0} ms, sequence: {1}", NetworkManager.m_Latency, NetworkManager.m_Sequence);
                m_LatencyText.text = string.Format ("ping: {0} ms, sequence: {1}", 0, 0);
                yield return wait;
            }
        }

        void Connect ()
        {
            NetworkManager.ComposeOutput (new NC_ServerLogin { m_ID = 1 });
        }

        void CreateGame ()
        {
            NetworkManager.ComposeOutput (new NC_ServerCreateGame ());
        }

        void ShutdownServer ()
        {
            NetworkManager.ComposeOutput (new NC_ServerShutdown ());
        }
    }
}
