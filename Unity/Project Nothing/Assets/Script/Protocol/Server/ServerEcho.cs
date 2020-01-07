using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    sealed class ServerEcho : NetProtocol<ServerEcho>
    {
        public string kString;

        public override void Deserialize (InStream inStream)
        {
        }

        public override void Excute ()
        {
        }

        public override void Serialize (OutStream outStream)
        {
            outStream.WriteString (kString);
        }
    }
}
