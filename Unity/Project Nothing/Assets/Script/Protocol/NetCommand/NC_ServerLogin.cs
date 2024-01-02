namespace ProjectNothing
{
    public sealed class NC_ServerLogin : NetCommand<NC_ServerLogin>
    {
        public int m_ID;

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_ID);
        }
    }
}
