using ProjectNothing.Protocol;
using System.Collections.Generic;

namespace ProjectNothing.Network
{
    public sealed class ProtocolManager : MonoSingleton<ProtocolManager>
    {
        private readonly Dictionary<ushort, INetProtocolGenerator> m_ProtocolMap = new Dictionary<ushort, INetProtocolGenerator> ();

        public void Init ()
        {
            RegisterNetCommand<NC_ServerLogin> (1);
            RegisterNetCommand<NC_ServerUdpConnect> (2);
            RegisterNetEvent<NE_ClientLoginResult> (51);
            RegisterNetEvent<NE_ClientUdpConnectResult> (52);

            RegisterNetCommand<NC_ServerEcho> (100);
            RegisterNetEvent<NE_ClientEchoResult> (200);
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
                m_ProtocolMap.Add (protocolID, new NetCommandGenerator<T> ());
            }
        }

        private void RegisterNetEvent<T> (ushort protocolID) where T : NetEvent<T>, new()
        {
            NetEvent<T>.m_ProtocolID = protocolID;

            if (!m_ProtocolMap.ContainsKey (protocolID))
            {
                m_ProtocolMap.Add (protocolID, new NetEventGenerator<T> ());
            }
        }
    }
}
