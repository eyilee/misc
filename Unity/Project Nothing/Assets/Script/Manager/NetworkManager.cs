using System.Collections;
using System.Diagnostics;
using System.Net;

namespace ProjectNothing
{
    public sealed class NetworkManager
    {
        public static uint m_ID = 0;
        public static uint m_Key = 0;

        static readonly TcpConnection m_TcpConnection = new ();
        static readonly UdpConnection m_UdpConnection = new ();

        static readonly Stopwatch m_StopWatch = new ();
        static long m_NextEchoTime = 0;
        static ushort m_LastSequence = 0;

        static bool m_IsInit = false;

        public static long m_Latency = 0;

        public static IPEndPoint GetUdpIPEndPoint () { return m_UdpConnection.GetIPEndPoint (); }

        public static IEnumerator Init (string host, int tcpPort, int udpPort)
        {
            if (!IPAddress.TryParse (host, out IPAddress ipAddress))
            {
                yield break;
            }

            yield return m_TcpConnection.Init (ipAddress, tcpPort);
            yield return m_UdpConnection.Init (ipAddress, udpPort);

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
        }

        public static void RefreshLatency (ushort sequence, long sendTime)
        {
            if (sequence == m_LastSequence)
            {
                long time = m_StopWatch.ElapsedMilliseconds;
                m_Latency = (time - sendTime) / 2;
            }
        }

        public static void ComposeTcpOutput (INetProtocol protocol)
        {
            m_TcpConnection.ComposeOutput (protocol);
        }

        public static void ComposeUdpOutput (INetProtocol protocol)
        {
            m_UdpConnection.ComposeOutput (protocol);
        }
    }
}
