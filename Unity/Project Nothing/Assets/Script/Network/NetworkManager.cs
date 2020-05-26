using System;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing.Network
{
    public sealed class NetworkManager : MonoSingleton<NetworkManager>
    {
        private readonly TcpClient m_TcpClient = new TcpClient ();
        private NetworkStream m_NetworkStream;

        public readonly NetBridge m_NetBridge = new NetBridge ();

        private readonly byte[] m_SendBuffer = new byte[1024];
        private readonly byte[] m_ReceiveBuffer = new byte[1024];

        public void Init (string host, int port)
        {
            m_TcpClient.BeginConnect (host, port, OnConnect, null);
        }

        private void OnConnect (IAsyncResult asyncResult)
        {
            m_TcpClient.EndConnect (asyncResult);

            if (m_TcpClient.Connected)
            {
                m_NetworkStream = m_TcpClient.GetStream ();

                AsyncRead ();

                Debug.Log ("Server connected.");
            }
        }

        private void AsyncRead ()
        {
            if (m_NetworkStream.CanRead)
            {
                m_NetworkStream.BeginRead (m_ReceiveBuffer, 0, m_ReceiveBuffer.Length, (IAsyncResult asyncResult) =>
                {
                    int length = m_NetworkStream.EndRead (asyncResult);
                    OnRead (length);
                    AsyncRead ();
                }, null);
            }
        }

        private void OnRead (int length)
        {
            InStream inStream = new InStream (m_ReceiveBuffer, length);
            m_NetBridge.ResolveInput (inStream);
        }

        private void AsyncWrite (int length)
        {
            if (m_NetworkStream.CanWrite)
            {
                m_NetworkStream.BeginWrite (m_SendBuffer, 0, length, (IAsyncResult asyncResult) =>
                {
                    m_NetworkStream.EndWrite (asyncResult);
                }, null);
            }
        }

        public void OnWrite (OutStream outStream)
        {
            byte[] data = outStream.Data ();
            Array.Copy (data, m_SendBuffer, data.Length);
            AsyncWrite (data.Length);
        }
    }
}
