using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class UdpSession
    {
        public const int UDP_SOCKET_BUFFER_SIZE = 512;

        class SendCommand
        {
            public byte[] m_Bytes;

            public SendCommand (byte[] bytes)
            {
                m_Bytes = new byte[bytes.Length];
                bytes.CopyTo (m_Bytes, 0);
            }
        }

        private UdpClient m_UdpClient = null;
        private IPEndPoint m_LocalIPEndPoint = null;
        private IPEndPoint m_RemoteIPEndPoint = null;

        private readonly LinkedList<SendCommand> m_SendQueue = new ();

        public IPEndPoint GetIPEndPoint () { return  m_LocalIPEndPoint; }

        public IEnumerator Init (IPAddress ipAddress, int port)
        {
            m_UdpClient = new UdpClient (0);
            m_LocalIPEndPoint = m_UdpClient.Client.LocalEndPoint as IPEndPoint;
            m_RemoteIPEndPoint = new IPEndPoint (ipAddress, port);

            AsyncReceive ();

            yield break;
        }

        private void AsyncReceive ()
        {
            m_UdpClient.BeginReceive ((IAsyncResult asyncResult) =>
            {
                byte[] bytes = m_UdpClient.EndReceive (asyncResult, ref m_RemoteIPEndPoint);
                OnReceive (bytes);
                AsyncReceive ();
            }, null);
        }

        private void OnReceive (byte[] bytes)
        {
            BitInStream inStream = new (bytes);
            inStream.Read (out uint key);

            if (NetworkManager.m_Key == key)
            {
                NetworkManager.ResolveInput (inStream);
            }
        }

        private void AsyncSend ()
        {
            if (m_SendQueue.First == null)
            {
                return;
            }

            SendCommand command = m_SendQueue.First ();

            m_UdpClient.BeginSend (command.m_Bytes, command.m_Bytes.Length, m_RemoteIPEndPoint, (IAsyncResult asyncResult) =>
            {
                m_UdpClient.EndSend (asyncResult);

                m_SendQueue.RemoveFirst ();

                AsyncSend ();
            }, null);
        }

        public void OnSend (BitOutStream outStream)
        {
            int size = outStream.GetSize ();
            if (size == 0 || size > UDP_SOCKET_BUFFER_SIZE)
            {
                Debug.LogErrorFormat ("Bytes size({0}) is 0 or more than UDP_SOCKET_BUFFER_SIZE({1}).", size, UDP_SOCKET_BUFFER_SIZE);
                return;
            }

            bool isSending = m_SendQueue.Count > 0;

            m_SendQueue.AddLast (new SendCommand (outStream.GetBytes ()));

            if (!isSending)
            {
                AsyncSend ();
            }
        }
    }
}
