using ProjectNothing.Protocol;
using System.Collections.Generic;

namespace ProjectNothing.Network
{
    public sealed class ProtocolManager : MonoSingleton<ProtocolManager>
    {
        private readonly Dictionary<ushort, INetProtocolGenerator> m_ProtocolMap = new Dictionary<ushort, INetProtocolGenerator> ();

        public void Init ()
        {
            RegisterProtocol<ServerLogin> (1);
            RegisterProtocol<ServerUdpConnect> (2);
            RegisterProtocol<ClientLoginResult> (51);
            RegisterProtocol<ClientUdpConnectResult> (52);

            RegisterProtocol<ServerEcho> (100);
            RegisterProtocol<ClientEcho> (200);
        }

        static public INetProtocol GenerateProtocol (ushort protocolID)
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

        private void RegisterProtocol<T> (ushort protocolID) where T : NetProtocol<T>, new()
        {
            NetProtocol<T>.m_ProtocolID = protocolID;

            if (!m_ProtocolMap.ContainsKey (protocolID))
            {
                m_ProtocolMap.Add (protocolID, new NetProtocolGenerator<T> ());
            }
        }
    }
}
