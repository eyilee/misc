using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProjectNothing
{
    public class GameOutPacket
    {

    }

    public sealed class GameConnection : UdpConnection<GameOutPacket>
    {
        public GameConnection ()
        {
        }

        public override void ResolvePackage (BitInStream inStream)
        {
            inStream.Read (out uint message);

            //if ((message & static_cast<uint32_t> (EGameMessage::Command)) > 0)
            //{
            //    ResolveSnapshot (inStream);
            //}
        }

        public void ResolveSnapshot (BitInStream inStream)
        {
        }

        public override void OnPacketAcked (uint sequence, GameOutPacket outPacket)
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

            // TODO: write snapshot

            EndComposeOutput (outStream);
        }
    }
}
