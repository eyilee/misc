using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class GameManager : MonoBehaviour
    {
        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public IEnumerator Start ()
        {
            yield return NetworkManager.Initialize (gameObject);

            if (NetworkManager.Instance != null)
            {
                Debug.Log ("NetworkManager exist.");
            }

            yield return SelectionManager.Initialize (gameObject);

            if (SelectionManager.Instance != null)
            {
                Debug.Log ("SelectionManager exist.");
            }
        }
    }
}