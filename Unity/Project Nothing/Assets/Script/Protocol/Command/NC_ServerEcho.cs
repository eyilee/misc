using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class NC_ServerEcho : NetCommand<NC_ServerEcho>
    {
        public string m_String;

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_String);
        }
    }
}
