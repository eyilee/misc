using System;
using System.Collections.Generic;

namespace SproutLands
{
    public static class ServiceLocator
    {
        private static readonly Dictionary<Type, ServiceBase> _services = new ();

        public static void Register<TService> (TService service) where TService : Service<TService>
        {
            Type type = typeof (TService);

            if (_services.ContainsKey (type))
            {
                throw new InvalidOperationException ($"Service already registered: {type}");
            }

            _services.Add (type, service);
        }

        public static void Register<TService> () where TService : Service<TService>, new()
        {
            Register (new TService ());
        }

        public static TService Resolve<TService> () where TService : Service<TService>
        {
            Type type = typeof (TService);

            if (!_services.TryGetValue (type, out ServiceBase service))
            {
                throw new InvalidOperationException ($"Service not registered: {type}");
            }

            return (TService)service;
        }
    }
}
