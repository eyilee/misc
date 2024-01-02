namespace ProjectNothing
{
    public sealed class NC_ServerEcho : NetCommand<NC_ServerEcho>
    {
        public ushort m_Sequence;
        public long m_SendTime;

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_Sequence);
            outStream.Write (m_SendTime);
        }
    }
}
