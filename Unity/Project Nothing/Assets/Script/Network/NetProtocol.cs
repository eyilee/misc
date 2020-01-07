namespace ProjectNothing.Network
{
    abstract class INetProtocol
    {
        protected NetBridge netBridge;

        public void SetNetBridge (NetBridge value) { netBridge = value; }

        public abstract void OnSerialize (OutStream outStream);
        public abstract void Serialize (OutStream outStream);
        public abstract void Deserialize (InStream inStream);
        public abstract void Excute ();
    }

    abstract class NetProtocol<T> : INetProtocol where T : NetProtocol<T>
    {
        public static ushort protocolID;

        public override void OnSerialize (OutStream outStream)
        {
            outStream.WriteUnsignedShort (protocolID);
            Serialize (outStream);
        }

        public override abstract void Serialize (OutStream outStream);
        public override abstract void Deserialize (InStream inStream);
        public override abstract void Excute ();
    }
}
