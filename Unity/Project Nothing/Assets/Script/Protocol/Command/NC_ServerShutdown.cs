using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class NC_ServerShutdown : NetCommand<NC_ServerShutdown>
    {
        public override void Serialize (BitOutStream outStream)
        {
        }
    }
}
