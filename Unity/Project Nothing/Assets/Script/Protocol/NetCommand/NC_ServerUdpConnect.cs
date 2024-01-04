namespace ProjectNothing
{
    public sealed class NC_ServerUdpConnect : NetCommand<NC_ServerUdpConnect>
    {
        public short m_nUdpPort;

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_nUdpPort);
        }
    }
}
