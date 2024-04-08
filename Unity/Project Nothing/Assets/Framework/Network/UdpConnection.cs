using System.Net;
using UnityEngine;

namespace ProjectNothing
{
    public struct FragmentHeader : IBitSerializable
    {
        public int m_Sequence;
        public int m_Count;
        public int m_Index;
        public int m_Size;

        public void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_Sequence);
            inStream.Read (out m_Count);
            inStream.Read (out m_Index);
            inStream.Read (out m_Size);
        }

        public readonly void Serialize (BitOutStream outStream)
        {
            outStream.Write (m_Sequence);
            outStream.Write (m_Count);
            outStream.Write (m_Index);
            outStream.Write (m_Size);
        }
    };

    public class FragmentReassembly
    {
        public const int REASSEMBLY_SIZE = 1024 * 32;

        public int m_Count;
        public int m_ReceivedMask;
        public int m_ReceivedCount;
        public int m_ReceivedSize;
        public byte[] m_Bytes = new byte[REASSEMBLY_SIZE];
    };

    public struct UdpHeader : IBitSerializable
    {
        public int m_Sequence;
        public int m_Ack;
        public int m_AckBits;

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
        public const int PACKET_BUFFER_SIZE = 256;
        public const int FRAGMENT_BUFFER_SIZE = 32;
        public const int FRAGMENT_SIZE = 10; //1420; // 1500(Ethernet MTU) - 60(IPv4 header) - 8(udp header) - 12(fragment header)

        #region Field
        protected UdpSession m_UdpSession = null;

        protected int m_InSequence = 0;
        protected int m_InAckBits = 0;
        protected int m_OutSequence = 1;
        protected int m_OutAck = 0;
        protected int m_OutAckBits = 0;
        protected readonly SequenceBuffer<TOutPacket> m_OutPackets = new (PACKET_BUFFER_SIZE);
        protected readonly SequenceBuffer<FragmentReassembly> m_FragmentReassembly = new (FRAGMENT_BUFFER_SIZE);
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
            //inStream.Read (out uint key);

            //if (key != m_UdpSession.Key)
            //{
            //    return;
            //}

            int sequence = ResolveHeader (inStream);
            if (sequence == 0)
            {
                return;
            }

            Debug.LogFormat ("Insequence: {0}", sequence);

            ResolvePackage (inStream);
        }

        public abstract void ResolvePackage (BitInStream inStream);

        public abstract void OnPacketAcked (int sequence, TOutPacket outPacket);

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
            int size = outStream.GetSize ();
            if (size > FRAGMENT_SIZE)
            {
                int fragmentCount = size / FRAGMENT_SIZE;
                int lastFragmentSize = size % FRAGMENT_SIZE;
                if (lastFragmentSize != 0)
                {
                    fragmentCount++;
                }
                else
                {
                    lastFragmentSize = FRAGMENT_SIZE;
                }

                for (int i = 0; i < fragmentCount; i++)
                {
                    int fragmentSize = i < fragmentCount - 1 ? FRAGMENT_SIZE : lastFragmentSize;

                    FragmentHeader header;
                    header.m_Sequence = m_OutSequence;
                    header.m_Count = fragmentCount;
                    header.m_Index = i;
                    header.m_Size = fragmentSize;

                    byte[] bytes = outStream.GetBytes ();

                    BitOutStream fragmentStream = new ();
                    fragmentStream.Write ((byte)1);
                    fragmentStream.Write (ref header);
                    fragmentStream.WriteBytes (ref bytes, i * FRAGMENT_SIZE, fragmentSize);
                    m_UdpSession.Send (fragmentStream);
                }
            }
            else
            {
                m_UdpSession.Send (outStream);
            }

            // NOTE: in an extreme case sequence may overflow
            m_OutSequence++;
        }

        protected int ResolveHeader (BitInStream inStream)
        {
            inStream.Read (out byte isFragment);

            if (isFragment == 1)
            {
                FragmentHeader fragmentHeader = new ();
                inStream.Read (ref fragmentHeader);

                FragmentReassembly reassembly = m_FragmentReassembly.TryGet (fragmentHeader.m_Sequence);
                if (reassembly == null)
                {
                    reassembly = m_FragmentReassembly.Insert (fragmentHeader.m_Sequence);
                    reassembly.m_Count = fragmentHeader.m_Count;
                    reassembly.m_ReceivedMask = 0;
                    reassembly.m_ReceivedCount = 0;
                    reassembly.m_ReceivedSize = 0;
                }

                if ((reassembly.m_ReceivedMask & (1 << fragmentHeader.m_Index)) != 0)
                {
                    return 0;
                }

                reassembly.m_ReceivedMask |= 1 << fragmentHeader.m_Index;
                reassembly.m_ReceivedCount++;
                reassembly.m_ReceivedSize += fragmentHeader.m_Size;

                inStream.ReadBytes (ref reassembly.m_Bytes, fragmentHeader.m_Index * FRAGMENT_SIZE, fragmentHeader.m_Size);

                if (reassembly.m_ReceivedCount < fragmentHeader.m_Count)
                {
                    return 0;
                }

                inStream = new BitInStream (reassembly.m_Bytes, reassembly.m_ReceivedSize);
            }

            UdpHeader header = new ();
            inStream.Read (out isFragment); // TODO: fix
            inStream.Read (ref header);

            int newInSequence = header.m_Sequence;
            int newOutAck = header.m_Ack;
            int newOutAckBits = header.m_AckBits;

            if (newInSequence > m_InSequence)
            {
                int distance = newInSequence - m_InSequence;
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
                int distance = m_InSequence - newInSequence;
                if (distance > 31)
                {
                    return 0;
                }

                int ackBit = 1 << distance;
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

            int latestLostSequence = m_OutAck >= 31 ? m_OutAck - 31 : 0;
            int oldestLostSequence = newOutAck >= 31 ? newOutAck - 31 : 0;
            for (int sequence = oldestLostSequence; sequence < latestLostSequence; sequence++)
            {
                int distance = m_OutAck >= sequence ? m_OutAck - sequence : 0;
                int ackBit = 1 << distance;
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

            int latestSequence = m_OutAck;
            int oldestSequence = m_OutAck >= 31 ? m_OutAck - 31 : 0;
            for (int sequence = oldestSequence; sequence <= latestSequence; sequence++)
            {
                int distance = m_OutAck - sequence;
                int ackBit = 1 << distance;
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
            UdpHeader header = new ()
            {
                m_Sequence = m_OutSequence,
                m_Ack = m_InSequence,
                m_AckBits = m_InAckBits
            };

            outStream.Write ((byte)0);
            outStream.Write (ref header);
        }
    }
}