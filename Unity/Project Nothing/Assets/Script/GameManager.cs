using ProjectNothing.Network;
using System.Collections;

namespace ProjectNothing
{
    public class GameManager : MonoSingleton<GameManager>
    {
        private bool m_IsInit = false;

        public void Awake ()
        {
            DontDestroyOnLoad (gameObject);
        }

        public IEnumerator Start ()
        {
            Initialize (gameObject);

            ProtocolManager.Init ();

            yield return NetworkManager.Init ("127.0.0.1", 8484, 8485);

            m_IsInit = true;
        }

        public void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            NetworkManager.Update ();
        }

        public void CreateGame ()
        {

        }
    }
}