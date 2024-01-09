namespace ProjectNothing
{
    public sealed class NE_ClientJoinGame : NetEvent<NE_ClientJoinGame>
    {
        public uint m_GameID;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_GameID);
        }

        public override void Excute ()
        {
        }
    }
}
