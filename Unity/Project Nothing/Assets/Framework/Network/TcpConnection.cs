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

        public IEnumerator Init (IPAddress ipAddress, ushort port)
        {
            yield return m_TcpSession.Init (this, ipAddress, port);

            m_IsInit = true;
        }

        public void Shutdown ()
        {
            if (!m_IsInit)
            {
                return;
            }

            m_TcpSession.Shutdown ();

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

            BitOutStream outStream = new ();
            protocol.OnSerialize (outStream);
            m_TcpSession.Write (outStream);
        }
    }
}