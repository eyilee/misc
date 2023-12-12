using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class ServerLogin : NetProtocol<ServerLogin>
    {
        public int m_ID;
        public int m_Key;

        public override void Deserialize (BitInStream inStream)
        {
        }

        public override void Excute ()
        {
        }

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_ID);
            outStream.Write (m_Key);
        }
    }
}
