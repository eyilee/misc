using UnityEngine;

namespace ProjectNothing
{
    public class MonoSingleton<T> : MonoBehaviour where T : MonoSingleton<T>
    {
        private static T m_Instance = null;

        public static T Instance {
            get {
                return m_Instance;
            }
        }

        public static void Initialize (GameObject gameObject)
        {
            T instance = FindObjectOfType<T> ();

            if (instance == null)
            {
                m_Instance = gameObject.AddComponent<T> ();
            }
            else
            {
                if (ReferenceEquals (instance.gameObject, gameObject))
                {
                    m_Instance = instance;
                }
                else
                {
                    Destroy (instance);
                    m_Instance = gameObject.AddComponent<T> ();
                }
            }
        }
    }
}
