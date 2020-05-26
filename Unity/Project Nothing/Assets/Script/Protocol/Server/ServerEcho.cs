using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class ServerEcho : NetProtocol<ServerEcho>
    {
        public string m_String;

        public override void Deserialize (InStream inStream)
        {
        }

        public override void Excute ()
        {
        }

        public override void Serialize (OutStream outStream)
        {
            outStream.WriteString (m_String);
        }
    }
}
