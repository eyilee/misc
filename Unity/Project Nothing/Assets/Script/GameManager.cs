using ProjectNothing.Network;
using ProjectNothing.Protocol;
using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class GameManager : MonoBehaviour
    {
        float time = 0.0f;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public void Update ()
        {
            time += Time.deltaTime;

            if (time >= 3.0f)
            {
                NetworkManager.Instance.netBridge.ComposeOutput (new ServerEcho { kString = "Hello World!" });
                time = 0.0f;
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
        }
    }
}