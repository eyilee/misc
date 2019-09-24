using System.Collections;
using UnityEngine;

namespace ProjectNothing
{
    public class MonoSingleton<T> : MonoBehaviour where T : MonoSingleton<T>
    {
        private static T m_instance = null;

        public static T Instance {
            get {
                return m_instance;
            }
        }

        public static IEnumerator Initialize (GameObject gameObject)
        {
            T instance = FindObjectOfType<T> ();

            if (instance == null)
            {
                instance = gameObject.AddComponent<T> ();
            }

            m_instance = instance;

            yield return null;
        }
    }
}
