using System.Collections;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Net;

namespace ProjectNothing
{
    public sealed class NetworkManager
    {
        private static ConcurrentQueue<INetProtocol> m_InputProtocols;

        private static IPAddress m_IPAddress = null;
        private static TcpSession m_TcpSession = null;
        private static UdpSession m_UdpSession = null;

        public static bool m_IsInit = false;

        public static uint m_ID;
        public static uint m_Key;

        private static Stopwatch m_StopWatch = null;
        private static long m_NextEchoTime = 0;
        private static ushort m_LastSequence = 0;

        public static long m_Latency = 0;

        public static IEnumerator Init (string host, int tcpPort, int udpPort)
        {
            m_InputProtocols = new ConcurrentQueue<INetProtocol> ();

            if (!IPAddress.TryParse (host, out m_IPAddress))
            {
                yield break;
            }

            m_TcpSession = new TcpSession ();
            yield return m_TcpSession.Init (m_IPAddress, tcpPort);

            m_UdpSession = new UdpSession ();
            yield return m_UdpSession.Init (m_IPAddress, udpPort);

            m_StopWatch = new Stopwatch ();
            m_StopWatch.Start ();

            m_IsInit = true;
        }

        public static void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            long time = m_StopWatch.ElapsedMilliseconds;
            if (time >= m_NextEchoTime)
            {
                m_NextEchoTime = time + 1000;

                m_LastSequence++;
                ComposeUdpOutput (new NC_ServerEcho { m_Sequence = m_LastSequence, m_SendTime = time });
            }

            foreach (INetProtocol protocol in m_InputProtocols)
            {
                protocol.Excute ();
            }

            m_InputProtocols.Clear ();
        }

        public static void RefreshLatency (ushort sequence, long sendTime)
        {
            if (sequence == m_LastSequence)
            {
                long time = m_StopWatch.ElapsedMilliseconds;
                m_Latency = (time - sendTime) / 2;
            }
        }

        public static void ResolveInput (BitInStream inStream)
        {
            inStream.Read (out ushort protocolID);

            INetProtocol protocol = ProtocolManager.GenerateProtocol (protocolID);
            if (protocol != null)
            {
                protocol.Deserialize (inStream);
                m_InputProtocols.Enqueue (protocol);
            }
        }

        public static void ComposeTcpOutput (INetProtocol protocol)
        {
            BitOutStream outStream = new ();
            protocol.OnSerialize (outStream);
            m_TcpSession.OnWrite (outStream);
        }

        public static void ComposeUdpOutput (INetProtocol protocol)
        {
            BitOutStream outStream = new ();
            outStream.Write (m_ID);
            outStream.Write (m_Key);
            protocol.OnSerialize (outStream);
            m_UdpSession.OnSend (outStream);
        }
    }
}
