namespace ProjectNothing.Network
{
    public abstract class INetProtocol
    {
        protected NetBridge m_NetBridge;

        public void SetNetBridge (NetBridge value) { m_NetBridge = value; }

        public abstract void OnSerialize (OutStream outStream);
        public abstract void Serialize (OutStream outStream);
        public abstract void Deserialize (InStream inStream);
        public abstract void Excute ();
    }

    public abstract class NetProtocol<T> : INetProtocol where T : NetProtocol<T>
    {
        public static ushort m_ProtocolID;

        public override void OnSerialize (OutStream outStream)
        {
            outStream.WriteUnsignedShort (m_ProtocolID);
            Serialize (outStream);
        }

        public override abstract void Serialize (OutStream outStream);
        public override abstract void Deserialize (InStream inStream);
        public override abstract void Excute ();
    }
}
