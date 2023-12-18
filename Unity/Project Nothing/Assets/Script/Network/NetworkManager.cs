using System.Collections;
using System.Net;

namespace ProjectNothing.Network
{
    public sealed class NetworkManager : MonoSingleton<NetworkManager>
    {
        private static IPAddress m_IPAddress = null;
        private static readonly TcpSession m_TcpSession = new ();
        private static readonly UdpSession m_UdpSession = new ();

        public static int m_ID;
        public static uint m_Key;

        public static IEnumerator Init (string host, int tcpPort, int udpPort)
        {
            if (!IPAddress.TryParse (host, out m_IPAddress))
            {
                yield break;
            }

            yield return m_TcpSession.Init (m_IPAddress, tcpPort);
            yield return m_UdpSession.Init (m_IPAddress, udpPort);
        }

        public static void ResolveInput (BitInStream inStream)
        {
            inStream.Read (out ushort protocolID);

            INetProtocol protocol = ProtocolManager.GenerateProtocol (protocolID);
            if (protocol != null)
            {
                protocol.Deserialize (inStream);
                protocol.Excute ();
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
