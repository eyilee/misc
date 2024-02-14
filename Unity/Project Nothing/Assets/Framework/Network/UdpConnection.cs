using System.Net;

namespace ProjectNothing
{
    struct UdpHeader : IBitSerializable
    {
        public uint m_Sequence;
        public uint m_Ack;
        public uint m_AckBits;

        public void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_Sequence);
            inStream.Read (out m_Ack);
            inStream.Read (out m_AckBits);
        }

        public readonly void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_Sequence);
            outStream.Write (m_Ack);
            outStream.Write (m_AckBits);
        }
    }

    public abstract class IUdpConnection
    {
        public abstract void Connect (IPAddress ipAddress, ushort port, uint key);
        public abstract void Init ();
        public abstract void Shutdown ();
        public abstract void OnDisconnect ();

        public abstract void ResolveInput (BitInStream inStream);
    }

    public abstract class UdpConnection<TOutPacket> : IUdpConnection where TOutPacket : class, new()
    {
        public const int SEQUENCE_BUFFER_SIZE = 256;

        #region Field
        protected UdpSession m_UdpSession = null;

        protected uint m_InSequence = 0;
        protected uint m_InAckBits = 0;
        protected uint m_OutSequence = 1;
        protected uint m_OutAck = 0;
        protected uint m_OutAckBits = 0;
        protected readonly SequenceBuffer<TOutPacket> m_OutPackets = new (SEQUENCE_BUFFER_SIZE);
        #endregion

        #region Property
        public ushort Port => m_UdpSession.Port;
        #endregion

        public UdpConnection ()
        {
            m_UdpSession = new UdpSession ();
        }

        public override void Connect (IPAddress ipAddress, ushort port, uint key)
        {
            m_UdpSession.Connect (ipAddress, port, key);
        }

        public override void Init ()
        {
            m_UdpSession.Init (this);
        }

        public override void Shutdown ()
        {
            m_UdpSession.Shutdown ();
            m_UdpSession = null;
        }

        public override void OnDisconnect ()
        {
            m_UdpSession = null;
        }

        public override void ResolveInput (BitInStream inStream)
        {
            inStream.Read (out uint key);

            if (key != m_UdpSession.Key)
            {
                return;
            }

            uint sequence = ResolveHeader (inStream);
            if (sequence == 0)
            {
                return;
            }

            ResolvePackage (inStream);
        }

        public abstract void ResolvePackage (BitInStream inStream);

        public abstract void OnPacketAcked (uint sequence, TOutPacket outPacket);

        public bool CanComposeOutput ()
        {
            if (m_OutPackets.IsExist (m_OutSequence))
            {
                // TODO: need server send data to update ack
                return false;
            }

            return true;
        }

        public void BeginComposeOutput (BitOutStream outStream)
        {
            outStream.Write (m_UdpSession.Key);

            ComposeHeader (outStream);
        }

        public void EndComposeOutput (BitOutStream outStream)
        {
            m_UdpSession.Send (outStream);

            // NOTE: in an extreme case sequence may overflow
            m_OutSequence++;
        }

        protected uint ResolveHeader (BitInStream inStream)
        {
            UdpHeader header = new ();
            inStream.Read (ref header);

            uint newInSequence = header.m_Sequence;
            uint newOutAck = header.m_Ack;
            uint newOutAckBits = header.m_AckBits;

            if (newInSequence > m_InSequence)
            {
                int distance = (int)(newInSequence - m_InSequence);
                if (distance > 31)
                {
                    m_InAckBits = 1;
                }
                else
                {
                    m_InAckBits <<= distance;
                    m_InAckBits |= 1;
                }

                m_InSequence = newInSequence;
            }
            else if (newInSequence < m_InSequence)
            {
                int distance = (int)(m_InSequence - newInSequence);
                if (distance > 31)
                {
                    return 0;
                }

                uint ackBit = (uint)(1 << distance);
                if ((m_InAckBits & ackBit) != 0)
                {
                    return 0;
                }

                m_InAckBits |= ackBit;
            }
            else
            {
                return 0;
            }

            if (newOutAck <= m_OutAck)
            {
                return newInSequence;
            }

            uint latestLostSequence = m_OutAck >= 31 ? m_OutAck - 31 : 0;
            uint oldestLostSequence = newOutAck >= 31 ? newOutAck - 31 : 0;
            for (uint sequence = oldestLostSequence; sequence < latestLostSequence; sequence++)
            {
                int distance = (int)(m_OutAck >= sequence ? m_OutAck - sequence : 0);
                uint ackBit = ((uint)(1 << distance));
                bool hasAcked = (m_OutAckBits & ackBit) != 0;

                if (m_OutPackets.IsExist (sequence) && !hasAcked)
                {
                    TOutPacket packet = m_OutPackets.TryGet (sequence);
                    if (packet != null)
                    {
                        OnPacketAcked (sequence, packet);
                    }

                    m_OutPackets.Remove (sequence);
                }
            }

            m_OutAck = newOutAck;
            m_OutAckBits = newOutAckBits;

            uint latestSequence = m_OutAck;
            uint oldestSequence = m_OutAck >= 31 ? m_OutAck - 31 : 0;
            for (uint sequence = oldestSequence; sequence <= latestSequence; sequence++)
            {
                int distance = (int)(m_OutAck - sequence);
                uint ackBit = (uint)(1 << distance);
                bool hasAcked = (m_OutAckBits & ackBit) != 0;

                if (m_OutPackets.IsExist (sequence) && !hasAcked)
                {
                    TOutPacket packet = m_OutPackets.TryGet (sequence);
                    if (packet != null)
                    {
                        OnPacketAcked (sequence, packet);
                    }

                    m_OutPackets.Remove (sequence);
                }
            }

            return newInSequence;
        }

        protected void ComposeHeader (BitOutStream outStream)
        {
            UdpHeader header;
            header.m_Sequence = m_OutSequence;
            header.m_Ack = m_InSequence;
            header.m_AckBits = m_InAckBits;

            outStream.Write (ref header);
        }
    }
}