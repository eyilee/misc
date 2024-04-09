using System.Collections.Concurrent;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class UdpSession
    {
        public const int UDP_SOCKET_BUFFER_SIZE = 1432; // 1500(Ethernet MTU) - 60(IPv4 header) - 8(udp header)

        class SendCommand
        {
            public byte[] m_Bytes;

            public SendCommand (byte[] bytes)
            {
                m_Bytes = new byte[bytes.Length];
                bytes.CopyTo (m_Bytes, 0);
            }
        }

        #region Field
        IUdpConnection m_Connection = null;
        uint m_Key = 0;
        UdpClient m_UdpClient = null;
        readonly ConcurrentQueue<SendCommand> m_SendQueue = new ();

        bool m_IsSending = false;
        #endregion

        #region Property
        public ushort Port => (ushort)(m_UdpClient.Client.LocalEndPoint as IPEndPoint).Port;
        public uint Key => m_Key;
        #endregion

        public UdpSession ()
        {
            m_UdpClient = new UdpClient (0);
        }

        public void Connect (IPAddress ipAddress, ushort port, uint key)
        {
            m_Key = key;
            m_UdpClient.Connect (ipAddress, port);
        }

        public void Init (IUdpConnection connection)
        {
            m_Connection = connection;

            AsyncReceive ();
        }

        public void Shutdown ()
        {
            m_UdpClient.Close ();
            m_UdpClient = null;

            m_Connection.OnDisconnect ();
            m_Connection = null;
        }

        async void AsyncReceive ()
        {
            UdpReceiveResult result = await m_UdpClient.ReceiveAsync ();
            OnReceive (result.Buffer);
            AsyncReceive ();
        }

        void OnReceive (byte[] bytes)
        {
            BitInStream inStream = new (bytes);
            m_Connection.ResolveInput (inStream);
        }

        async void AsyncSend ()
        {
            if (!m_SendQueue.TryDequeue (out SendCommand command))
            {
                m_IsSending = false;
                return;
            }

            await m_UdpClient.SendAsync (command.m_Bytes, command.m_Bytes.Length);

            AsyncSend ();
        }

        public void Send (BitOutStream outStream)
        {
            int size = outStream.GetSize ();
            if (size == 0 || size > UDP_SOCKET_BUFFER_SIZE)
            {
                Debug.LogErrorFormat ("Bytes size({0}) is 0 or more than UDP_SOCKET_BUFFER_SIZE({1}).", size, UDP_SOCKET_BUFFER_SIZE);
                return;
            }

            m_SendQueue.Enqueue (new SendCommand (outStream.GetBytes ()));

            if (!m_IsSending)
            {
                AsyncSend ();
            }
        }
    }
}
