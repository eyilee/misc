using System.Collections;
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

        public static IEnumerator Initialize (GameObject gameObject)
        {
            T instance = FindObjectOfType<T> ();

            if (instance == null)
            {
                instance = gameObject.AddComponent<T> ();
            }

            m_Instance = instance;

            yield return null;
        }
    }
}
