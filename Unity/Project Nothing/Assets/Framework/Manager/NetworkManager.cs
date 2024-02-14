using System.Collections;
using System.Net;

namespace ProjectNothing
{
    public sealed class NetworkManager
    {
        static IPAddress m_IPAddress = null;
        static TcpConnection m_Connection = null;

        //static readonly Stopwatch m_StopWatch = new ();
        //static long m_NextEchoTime = 0;
        //static ushort m_LastSequence = 0;

        static bool m_IsInit = false;

        //public static ushort m_Sequence = 0;
        //public static long m_Latency = 0;

        //public static IPEndPoint GetUdpIPEndPoint () { return m_UdpConnection.GetIPEndPoint (); }

        public static IEnumerator Init (string host, ushort port)
        {
            if (!IPAddress.TryParse (host, out m_IPAddress))
            {
                yield break;
            }

            m_Connection = new TcpConnection ();

            yield return m_Connection.Init (m_IPAddress, port);

            //m_StopWatch.Start ();

            m_IsInit = true;
        }

        public static void Shutdown ()
        {
            if (!m_IsInit)
            {
                return;
            }

            m_Connection.Shutdown ();

            //m_StopWatch.Stop ();

            m_IsInit = false;
        }

        public static void Update ()
        {
            if (!m_IsInit)
            {
                return;
            }

            m_Connection.Update ();

            //long time = m_StopWatch.ElapsedMilliseconds;
            //if (time >= m_NextEchoTime)
            //{
            //    m_NextEchoTime = time + 1000;

            //    m_LastSequence++;
            //    ComposeUdpOutput (new NC_ServerEcho { m_Sequence = m_LastSequence, m_SendTime = time });
            //}
        }

        public static void UdpConnect (IUdpConnection udpConnection, ushort port, uint key)
        {
            if (!m_IsInit)
            {
                return;
            }

            udpConnection.Connect (m_IPAddress, port, key);
        }

        //public static void RefreshLatency (ushort sequence, long sendTime)
        //{
        //    if (sequence == m_LastSequence)
        //    {
        //        m_Sequence = sequence;
        //        long time = m_StopWatch.ElapsedMilliseconds;
        //        m_Latency = (time - sendTime) / 2;
        //    }
        //}

        public static void ComposeOutput (INetProtocol protocol)
        {
            if (!m_IsInit)
            {
                return;
            }

            m_Connection.ComposeOutput (protocol);
        }
    }
}
