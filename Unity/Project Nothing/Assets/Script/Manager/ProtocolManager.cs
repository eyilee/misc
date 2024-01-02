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

            RegisterNetCommand<NC_ServerLogin> (1);
            RegisterNetCommand<NC_ServerUdpConnect> (2);
            RegisterNetEvent<NE_ClientLoginResult> (51);
            RegisterNetEvent<NE_ClientUdpConnectResult> (52);

            RegisterNetCommand<NC_ServerEcho> (100);
            RegisterNetCommand<NC_ServerCreateGame> (101);
            RegisterNetEvent<NE_ClientEchoResult> (200);
            RegisterNetEvent<NE_ClientCreateGameResult> (201);

            RegisterNetCommand<NC_ServerShutdown> (9000);
        }

        public static INetProtocol GenerateProtocol (ushort protocolID)
        {
            if (!m_ProtocolMap.TryGetValue (protocolID, out INetProtocolGenerator generator))
            {
                return null;
            }

            return generator.Generate ();
        }

        private static void RegisterNetCommand<T> (ushort protocolID) where T : NetCommand<T>, new()
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

        private static void RegisterNetEvent<T> (ushort protocolID) where T : NetEvent<T>, new()
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
