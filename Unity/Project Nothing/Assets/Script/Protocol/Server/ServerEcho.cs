using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class ServerEcho : NetProtocol<ServerEcho>
    {
        public string m_String;

        public override void Deserialize (BitInStream inStream)
        {
        }

        public override void Excute ()
        {
        }

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_String);
        }
    }
}
