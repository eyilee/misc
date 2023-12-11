namespace ProjectNothing.Network
{
    public abstract class INetProtocol
    {
        protected NetBridge m_NetBridge;

        public void SetNetBridge (NetBridge value) { m_NetBridge = value; }

        public abstract void OnSerialize (BitOutStream outStream);
        public abstract void Serialize (BitOutStream outStream);
        public abstract void Deserialize (BitInStream inStream);
        public abstract void Excute ();
    }

    public abstract class NetProtocol<T> : INetProtocol where T : NetProtocol<T>
    {
        public static ushort m_ProtocolID;

        public override void OnSerialize (BitOutStream outStream)
        {
            outStream.Write (m_ProtocolID);
            Serialize (outStream);
        }

        public override abstract void Serialize (BitOutStream outStream);
        public override abstract void Deserialize (BitInStream inStream);
        public override abstract void Excute ();
    }
}
