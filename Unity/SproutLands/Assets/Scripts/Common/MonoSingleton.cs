using System;
using UnityEngine;

namespace SproutLands
{
    public class MonoSingleton<T> : MonoBehaviour where T : MonoSingleton<T>
    {
        public static MonoSingleton<T>? Instance;

        protected virtual void Awake ()
        {
            if (Instance != null && Instance != this)
            {
                throw new InvalidOperationException ($"MonoSingleton {typeof (T)} can only have one instance.");
            }

            Instance = this;

            DontDestroyOnLoad (gameObject);
        }

        protected virtual void OnDestroy ()
        {
            Instance = null;
        }
    }
}
