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

        UdpConnection m_Connection = null;
        readonly UdpClient m_UdpClient = new (0);
        IPEndPoint m_RemoteIPEndPoint = null;

        readonly LinkedList<SendCommand> m_SendQueue = new ();

        public IPEndPoint GetIPEndPoint () { return m_UdpClient.Client.LocalEndPoint as IPEndPoint; }

        public void Init (UdpConnection connection, IPAddress ipAddress, ushort port)
        {
            m_Connection = connection;
            m_RemoteIPEndPoint = new IPEndPoint (ipAddress, port);

            AsyncReceive ();
        }

        public void Shutdown ()
        {
            m_UdpClient.Close ();
        }

        void AsyncReceive ()
        {
            m_UdpClient.BeginReceive ((IAsyncResult asyncResult) =>
            {
                byte[] bytes = m_UdpClient.EndReceive (asyncResult, ref m_RemoteIPEndPoint);
                OnReceive (bytes);
                AsyncReceive ();
            }, null);
        }

        void OnReceive (byte[] bytes)
        {
            BitInStream inStream = new (bytes);
            m_Connection.ResolveInput (inStream);
        }

        void AsyncSend ()
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

        public void Send (BitOutStream outStream)
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
