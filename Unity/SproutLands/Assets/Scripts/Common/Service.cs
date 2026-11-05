using System;

namespace SproutLands
{
    public abstract class ServiceBase
    {
    }

    public abstract class Service<T> : ServiceBase where T : Service<T>
    {
        public static T? Instance;

        public Service ()
        {
            if (Instance != null && Instance != this)
            {
                throw new InvalidOperationException ($"Service {typeof (T)} can only have one instance.");
            }

            Instance = this as T;
        }
    }
}
