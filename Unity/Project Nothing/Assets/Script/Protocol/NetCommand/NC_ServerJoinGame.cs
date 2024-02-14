namespace ProjectNothing
{
    public sealed class NC_ServerJoinGame : NetCommand<NC_ServerJoinGame>
    {
        public uint m_GameID;
        public ushort m_Port;

        public override void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_GameID);
            outStream.Write (m_Port);
        }
    }
}
