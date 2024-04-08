namespace ProjectNothing
{
    enum EGameMessage : uint
    {
        None = 0,
        Command = 1 << 0,
        Snapshot = 1 << 1
    };

    public class GameOutPacket
    {

    }

    public sealed class ClientConnection : UdpConnection<GameOutPacket>
    {
        public ClientConnection ()
        {
        }

        public override void ResolvePackage (BitInStream inStream)
        {
            inStream.Read (out uint message);

            if (((EGameMessage)message & EGameMessage.Snapshot) > 0)
            {
                ResolveSnapshot (inStream);
            }
        }

        public void ResolveSnapshot (BitInStream inStream)
        {
        }

        public override void OnPacketAcked (int sequence, GameOutPacket outPacket)
        {
            // outPacket.Reset ();
        }

        public void ComposePackage ()
        {
            if (!CanComposeOutput ())
            {
                return;
            }

            // SGameOutPacket & outPacket = m_kOutPackets.Insert (m_nOutSequence);

            BitOutStream outStream = new ();
            BeginComposeOutput (outStream);

            // TODO: write commands

            EndComposeOutput (outStream);
        }
    }
}
