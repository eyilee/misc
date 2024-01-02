namespace ProjectNothing
{
    public abstract class INetProtocol
    {
        public abstract void OnSerialize (BitOutStream outStream);
        public abstract void Serialize (BitOutStream outStream);
        public abstract void Deserialize (BitInStream inStream);
        public abstract void Excute ();
    }

    public abstract class NetCommand<T> : INetProtocol
    {
        public static ushort m_ProtocolID;

        public sealed override void OnSerialize (BitOutStream outStream)
        {
            outStream.Write (m_ProtocolID);
            Serialize (outStream);
        }

        public abstract override void Serialize (BitOutStream outStream);
        public sealed override void Deserialize (BitInStream inStream) { }
        public sealed override void Excute () { }
    }

    public abstract class NetEvent<T> : INetProtocol
    {
        public static ushort m_ProtocolID;

        public sealed override void OnSerialize (BitOutStream outStream) { }
        public sealed override void Serialize (BitOutStream outStream) { }
        public abstract override void Deserialize (BitInStream inStream);
        public abstract override void Excute ();
    }
}
