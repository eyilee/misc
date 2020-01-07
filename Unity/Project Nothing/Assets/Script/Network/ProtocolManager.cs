using System.Collections.Generic;
using ProjectNothing.Protocol;

namespace ProjectNothing.Network
{
    sealed class ProtocolManager : MonoSingleton<ProtocolManager>
    {
        private readonly Dictionary<ushort, INetProtocolGenerator> protocolMap = new Dictionary<ushort, INetProtocolGenerator> ();

        public void Init ()
        {
            RegisterProtocol<ServerEcho> (100);
            RegisterProtocol<ClientEcho> (200);
        }

        public INetProtocol GenerateProtocol (ushort protocolID, NetBridge netBridge)
        {
            INetProtocol netProtocol = protocolMap[protocolID].Generate ();
            netProtocol.SetNetBridge (netBridge);
            return netProtocol;
        }

        private void RegisterProtocol<T> (ushort protocolID) where T : NetProtocol<T>, new()
        {
            INetProtocolGenerator netProtocolGenerator = new NetProtocolGenerator<T> ();

            if (!protocolMap.ContainsKey (protocolID))
            {
                NetProtocol<T>.protocolID = protocolID;
                protocolMap.Add (protocolID, netProtocolGenerator);
            }
        }
    }
}
