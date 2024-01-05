using System.Collections;
using System.Collections.Concurrent;
using System.Net;

namespace ProjectNothing
{
    public sealed class TcpConnection
    {
        readonly TcpSession m_TcpSession = new ();
        readonly ConcurrentQueue<INetProtocol> m_QueuedProtocols = new ();

        bool m_IsInit = false;

        public IEnumerator Init (IPAddress ipAddress, int port)
        {
            yield return m_TcpSession.Init (this, ipAddress, port);

            m_IsInit = true;
        }

        public void Shutdown ()
        {
            m_TcpSession.Shutdown ();
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
            protocol.OnSerialize (outStream);
            m_TcpSession.Write (outStream);
        }
    }
}