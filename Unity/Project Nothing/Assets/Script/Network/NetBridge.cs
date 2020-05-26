namespace ProjectNothing.Network
{
    public sealed class NetBridge
    {
        public void ResolveInput (InStream inStream)
        {
            ushort protocolID;
            inStream.ReadUnsignedShort (out protocolID);

            INetProtocol netProtocol = GenerateProtocol (protocolID, this);
            netProtocol.Deserialize (inStream);
            netProtocol.Excute ();
        }

        public void ComposeOutput (INetProtocol netProtocol)
        {
            OutStream outStream = new OutStream ();
            netProtocol.OnSerialize (outStream);
            NetworkManager.Instance.OnWrite (outStream);
        }

        private INetProtocol GenerateProtocol (ushort protocolID, NetBridge netBridge)
        {
            return ProtocolManager.Instance.GenerateProtocol (protocolID, netBridge);
        }
    }
}
