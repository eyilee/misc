using ProjectNothing.Network;
using ProjectNothing.Protocol;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class MainUI : MonoBehaviour
{
    private Button m_CreateButton;
    private TextMeshProUGUI m_CreateText;

    private Button m_ShutdownButton;
    private TextMeshProUGUI m_ShutdownText;

    void Awake ()
    {
        m_CreateButton = transform.Find ("btn_create").GetComponent<Button> ();
        m_CreateButton.onClick.AddListener (CreateGame);

        m_CreateText = transform.Find ("btn_create/txt_create").GetComponent<TextMeshProUGUI> ();
        m_CreateText.text = "創建遊戲";

        m_ShutdownButton = transform.Find ("btn_shutdown").GetComponent<Button> ();
        m_ShutdownButton.onClick.AddListener (ShutdownServer);

        m_ShutdownText = transform.Find ("btn_shutdown/txt_shutdown").GetComponent<TextMeshProUGUI> ();
        m_ShutdownText.text = "關閉伺服器";
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
