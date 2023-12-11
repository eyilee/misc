namespace ProjectNothing.Network
{
    public sealed class NetBridge
    {
        public void ResolveInput (BitInStream inStream)
        {
            ushort protocolID;
            inStream.Read (out protocolID);

            INetProtocol netProtocol = GenerateProtocol (protocolID, this);
            netProtocol.Deserialize (inStream);
            netProtocol.Excute ();
        }

        public void ComposeOutput (INetProtocol netProtocol)
        {
            BitOutStream outStream = new BitOutStream ();
            netProtocol.OnSerialize (outStream);
            NetworkManager.Instance.OnWrite (outStream);
        }

        private INetProtocol GenerateProtocol (ushort protocolID, NetBridge netBridge)
        {
            return ProtocolManager.Instance.GenerateProtocol (protocolID, netBridge);
        }
    }
}
