using System.Collections.Generic;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class ProtocolManager
    {
        private static Dictionary<ushort, INetProtocolGenerator> m_ProtocolMap = null;

        public static void Init ()
        {
            m_ProtocolMap = new Dictionary<ushort, INetProtocolGenerator> ();
        }

        public static INetProtocol GenerateProtocol (ushort protocolID)
        {
            if (!m_ProtocolMap.TryGetValue (protocolID, out INetProtocolGenerator generator))
            {
                return null;
            }

            return generator.Generate ();
        }

        public static void RegisterNetCommand<T> (ushort protocolID) where T : NetCommand<T>, new()
        {
            NetCommand<T>.m_ProtocolID = protocolID;

            if (!m_ProtocolMap.ContainsKey (protocolID))
            {
                m_ProtocolMap.Add (protocolID, new NetProtocolGenerator<T> ());
            }
            else
            {
                Debug.LogErrorFormat ("NetCommand ID({0}) has registered.", protocolID);
            }
        }

        public static void RegisterNetEvent<T> (ushort protocolID) where T : NetEvent<T>, new()
        {
            NetEvent<T>.m_ProtocolID = protocolID;

            if (!m_ProtocolMap.ContainsKey (protocolID))
            {
                m_ProtocolMap.Add (protocolID, new NetProtocolGenerator<T> ());
            }
            else
            {
                Debug.LogErrorFormat ("NetEvent ID({0}) has registered.", protocolID);
            }
        }
    }
}
