namespace ProjectNothing
{
    public sealed class NE_ClientEchoResult : NetEvent<NE_ClientEchoResult>
    {
        public ushort m_Sequence;
        public long m_SendTime;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_Sequence);
            inStream.Read (out m_SendTime);
        }

        public override void Excute ()
        {
            //NetworkManager.RefreshLatency (m_Sequence, m_SendTime);
        }
    }
}
