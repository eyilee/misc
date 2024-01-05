using System.Collections;
using System.Collections.Concurrent;
using System.Net;

namespace ProjectNothing
{
    public sealed class UdpConnection
    {
        public const int SEQUENCE_BUFFER_SIZE = 256;

        struct Header : IBitSerializable
        {
            uint m_Sequence;
            uint m_Ack;
            uint m_AckBits;

            public void Deserialize (BitInStream inStream)
            {
                inStream.Read (out m_Sequence);
                inStream.Read (out m_Ack);
                inStream.Read (out m_AckBits);
            }

            public void Serialize (BitOutStream outStream)
            {
                outStream.Write (m_Sequence);
                outStream.Write (m_Ack);
                outStream.Write (m_AckBits);
            }
        }

        class InPacket
        {
            public bool m_Acked;
        }

        class OutPacket
        {
            public bool m_Reliable;
            public byte[] m_Bytes;
        }

        readonly UdpSession m_UdpSession = new ();
        readonly ConcurrentQueue<INetProtocol> m_QueuedProtocols = new ();

        uint m_InSequence;
        uint m_InAckBits;
        SequenceBuffer<InPacket> m_InPackets = new SequenceBuffer<InPacket> (SEQUENCE_BUFFER_SIZE);

        uint m_OutSequence;
        uint m_OutAck;
        uint m_OutAckBits;
        SequenceBuffer<OutPacket> m_OutPackets = new SequenceBuffer<OutPacket> (SEQUENCE_BUFFER_SIZE);

        bool m_IsInit = false;

        public IPEndPoint GetIPEndPoint () { return m_UdpSession.GetIPEndPoint (); }

        public IEnumerator Init (IPAddress ipAddress, int port)
        {
            yield return m_UdpSession.Init (this, ipAddress, port);

            m_IsInit = true;
        }

        public void Shutdown ()
        {
            m_UdpSession.Shutdown ();
        }

        public void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            foreach (INetProtocol protocol in m_QueuedProtocols)
            {
                protocol.Excute ();
            }

            m_QueuedProtocols.Clear ();
        }

        public void ResolveInput (BitInStream inStream)
        {
            inStream.Read (out uint id);
            inStream.Read (out uint key);

            if (id != NetworkManager.m_ID || key != NetworkManager.m_Key)
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
            BitOutStream outStream = new ();
            outStream.Write (NetworkManager.m_ID);
            outStream.Write (NetworkManager.m_Key);
            protocol.OnSerialize (outStream);
            m_UdpSession.Send (outStream);
        }

        void OnPacketAcked ()
        {

        }
    }
}