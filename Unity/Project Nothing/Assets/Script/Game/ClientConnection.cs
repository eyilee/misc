using UnityEngine;

namespace ProjectNothing
{
    public enum EGameMessage : uint
    {
        None = 0,
        Command = 1 << 0,
        Snapshot = 1 << 1
    };

    public class GameOutPacket
    {
        public void Reset ()
        {
            m_Message = EGameMessage.None;
        }

        public EGameMessage m_Message;
    }

    public class CommandInfo
    {
        public UserCommand m_Command;
    }

    public sealed class ClientConnection : UdpConnection<GameOutPacket>
    {
        public const int COMMAND_BUFFER_SIZE = 4;

        int m_CommandSequence = 0;
        int m_LastSentCommandSequence = 0;
        int m_CommandSequenceAck = 0;
        readonly SequenceBuffer<CommandInfo> m_OutCommands = new (COMMAND_BUFFER_SIZE);

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
            outPacket.Reset ();
        }

        public void ComposePackage ()
        {
            if (m_InSequence == 0 || !CanComposeOutput ())
            {
                return;
            }

            if (m_CommandSequence > 0 && m_CommandSequence <= m_LastSentCommandSequence)
            {
                return;
            }

            GameOutPacket outPacket = m_OutPackets.Insert (m_OutSequence);

            BitOutStream outStream = new ();
            BeginComposeOutput (outStream);

            int message = 0;
            message |= (int)EGameMessage.Command;
            outStream.Write (message);

            if (m_CommandSequence > 0)
            {
                m_LastSentCommandSequence = m_CommandSequence;
                ComposeCommand (outStream);
            }

            EndComposeOutput (outStream);
        }

        public void QueueCommand (uint tick, ref UserCommand command)
        {
            m_CommandSequence++;

            CommandInfo commandInfo = m_OutCommands.Insert (m_CommandSequence);
            commandInfo.m_Command = command;
        }

        void ComposeCommand (BitOutStream outStream)
        {
            int sequence = m_CommandSequence;
            outStream.Write (sequence);

            while (true)
            {
                CommandInfo commandInfo = m_OutCommands.TryGet (sequence--);
                if (commandInfo == null)
                {
                    break;
                }

                outStream.Write ((byte)1);
                outStream.Write (ref commandInfo.m_Command);
            }

            outStream.Write ((byte)0);
        }
    }
}
