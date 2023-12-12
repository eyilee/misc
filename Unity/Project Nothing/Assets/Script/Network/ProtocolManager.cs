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
            RegisterProtocol<ClientLoginResult> (50);

            RegisterProtocol<ServerEcho> (100);
            RegisterProtocol<ClientEcho> (200);
        }

        public INetProtocol GenerateProtocol (ushort protocolID, NetBridge netBridge)
        {
            INetProtocol netProtocol = m_ProtocolMap[protocolID].Generate ();
            netProtocol.SetNetBridge (netBridge);
            return netProtocol;
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
