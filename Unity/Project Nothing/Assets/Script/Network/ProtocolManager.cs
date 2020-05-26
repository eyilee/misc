using System.Collections.Generic;
using ProjectNothing.Protocol;

namespace ProjectNothing.Network
{
    public sealed class ProtocolManager : MonoSingleton<ProtocolManager>
    {
        private readonly Dictionary<ushort, INetProtocolGenerator> m_ProtocolMap = new Dictionary<ushort, INetProtocolGenerator> ();

        public void Init ()
        {
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
            INetProtocolGenerator netProtocolGenerator = new NetProtocolGenerator<T> ();

            if (!m_ProtocolMap.ContainsKey (protocolID))
            {
                NetProtocol<T>.m_ProtocolID = protocolID;
                m_ProtocolMap.Add (protocolID, netProtocolGenerator);
            }
        }
    }
}
