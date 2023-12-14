namespace ProjectNothing.Network
{
    public sealed class NetBridge
    {
        public void ResolveInput (BitInStream inStream)
        {
            inStream.Read (out ushort protocolID);

            INetProtocol protocol = GenerateProtocol (protocolID, this);
            if (protocol != null)
            {
                protocol.Deserialize (inStream);
                protocol.Excute ();
            }
        }

        public void ComposeOutput (INetProtocol protocol)
        {
            BitOutStream outStream = new ();
            protocol.OnSerialize (outStream);
            NetworkManager.Instance.OnWrite (outStream);
        }

        public void ComposeOutput (INetProtocol protocol, int id, uint key)
        {
            BitOutStream outStream = new ();
            outStream.Write (id);
            outStream.Write (key);
            protocol.OnSerialize (outStream);
            NetworkManager.Instance.OnSend (outStream);
        }

        private INetProtocol GenerateProtocol (ushort protocolID, NetBridge netBridge)
        {
            return ProtocolManager.Instance.GenerateProtocol (protocolID, netBridge);
        }
    }
}
