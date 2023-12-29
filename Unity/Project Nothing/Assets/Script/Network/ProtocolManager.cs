using ProjectNothing.Protocol;
using System.Collections.Generic;
using UnityEngine;

namespace ProjectNothing.Network
{
    public sealed class ProtocolManager : MonoSingleton<ProtocolManager>
    {
        private readonly Dictionary<ushort, INetProtocolGenerator> m_ProtocolMap = new ();

        public void Init ()
        {
            RegisterNetCommand<NC_ServerLogin> (1);
            RegisterNetCommand<NC_ServerUdpConnect> (2);
            RegisterNetEvent<NE_ClientLoginResult> (51);
            RegisterNetEvent<NE_ClientUdpConnectResult> (52);

            RegisterNetCommand<NC_ServerEcho> (100);
            RegisterNetCommand<NC_ServerCreateGame> (101);
            RegisterNetEvent<NE_ClientEchoResult> (200);

            RegisterNetCommand<NC_ServerShutdown> (9000);
        }

        public static INetProtocol GenerateProtocol (ushort protocolID)
        {
            if (Instance == null)
            {
                return null;
            }

            return Instance.Generate (protocolID);
        }

        private INetProtocol Generate (ushort protocolID)
        {
            if (!m_ProtocolMap.TryGetValue (protocolID, out INetProtocolGenerator generator))
            {
                return null;
            }

            return generator.Generate ();
        }

        private void RegisterNetCommand<T> (ushort protocolID) where T : NetCommand<T>, new()
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

        private void RegisterNetEvent<T> (ushort protocolID) where T : NetEvent<T>, new()
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
