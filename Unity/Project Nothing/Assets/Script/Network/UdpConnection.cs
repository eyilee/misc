using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Net;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class UdpConnection
    {
        public const int SEQUENCE_BUFFER_SIZE = 256;

        struct Header : IBitSerializable
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

        class OutPacket
        {
            public bool m_Reliable;
            public INetProtocol m_Protocol;

            public OutPacket ()
            {
                Reset ();
            }

            public void Reset ()
            {
                m_Reliable = false;
                m_Protocol = null;
            }
        }

        readonly UdpSession m_UdpSession = new ();
        readonly ConcurrentQueue<INetProtocol> m_QueuedProtocols = new ();

        uint m_InSequence = 0;
        uint m_InAckBits = 0;
        uint m_OutSequence = 1;
        uint m_OutAck = 0;
        uint m_OutAckBits = 0;
        readonly SequenceBuffer<OutPacket> m_OutPackets = new (SEQUENCE_BUFFER_SIZE);

        bool m_IsInit = false;

        public IPEndPoint GetIPEndPoint () { return m_UdpSession.GetIPEndPoint (); }

        public void Init (IPAddress ipAddress, ushort port)
        {
            m_UdpSession.Init (this, ipAddress, port);

            m_IsInit = true;
        }

        public void Shutdown ()
        {
            if (!m_IsInit)
            {
                return;
            }

            m_UdpSession.Shutdown ();

            m_IsInit = false;
        }

        public void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            while (m_QueuedProtocols.TryDequeue (out INetProtocol protocol))
            {
                protocol.Excute ();
            }
        }

        public void ResolveInput (BitInStream inStream)
        {
            if (!m_IsInit)
            {
                return;
            }

            inStream.Read (out uint id);
            inStream.Read (out uint key);

            if (id != NetworkManager.m_ID || key != NetworkManager.m_Key)
            {
                return;
            }

            uint sequence = ResolveHeader (inStream);
            if (sequence == 0)
            {
                return;
            }

            inStream.Read (out ushort protocolID);

            INetProtocol protocol = ProtocolManager.GenerateProtocol (protocolID);
            if (protocol != null)
            {
                protocol.Deserialize (inStream);
                m_QueuedProtocols.Enqueue (protocol);
            }
        }

        public void ComposeOutput (INetProtocol protocol)
        {
            if (!m_IsInit)
            {
                return;
            }

            if (m_OutPackets.IsExist (m_OutSequence))
            {
                // TODO: need client send data to update ack
                return;
            }

            Header header = new ()
            {
                m_Sequence = m_OutSequence,
                m_Ack = m_InSequence,
                m_AckBits = m_InAckBits
            };

            OutPacket outPacket = m_OutPackets.Insert (m_OutSequence);
            outPacket.m_Reliable = false;
            outPacket.m_Protocol = protocol;

            BitOutStream outStream = new ();
            outStream.Write (NetworkManager.m_ID);
            outStream.Write (NetworkManager.m_Key);
            outStream.Write (ref header);
            protocol.OnSerialize (outStream);
            m_UdpSession.Send (outStream);

            // NOTE: in an extreme case sequence may overflow
            m_OutSequence++;
        }

        uint ResolveHeader (BitInStream inStream)
        {
            Header header = new ();
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
                    OutPacket packet = m_OutPackets.TryGet (sequence);
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
                    OutPacket packet = m_OutPackets.TryGet (sequence);
                    if (packet != null)
                    {
                        OnPacketAcked (sequence, packet);
                    }

                    m_OutPackets.Remove (sequence);
                }
            }

            return newInSequence;
        }

        void OnPacketAcked (uint sequence, OutPacket outPacket)
        {
            outPacket.Reset ();
        }
    }
}