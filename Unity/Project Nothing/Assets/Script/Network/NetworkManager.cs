using System;
using System.Collections;
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

        private string m_Host = "127.0.0.1";
        private int m_Port = 8484;
        private bool m_IsInit = false;

        public IEnumerator Init (string host, int port)
        {
            m_Host = host;
            m_Port = port;
            m_TcpClient.BeginConnect (m_Host, m_Port, OnConnect, null);

            yield return new WaitUntil (() => m_IsInit);
        }

        private void OnConnect (IAsyncResult asyncResult)
        {
            if (m_TcpClient.Connected)
            {
                m_TcpClient.EndConnect (asyncResult);
                m_NetworkStream = m_TcpClient.GetStream ();

                AsyncRead ();

                m_IsInit = true;

                Debug.Log ("Server connected.");
            }
            else
            {
                Debug.Log ("Server connect failed.");
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
            BitInStream inStream = new BitInStream (m_ReceiveBuffer, length);
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

        public void OnWrite (BitOutStream outStream)
        {
            byte[] data = outStream.GetBytes ();
            Array.Copy (data, m_SendBuffer, data.Length);
            AsyncWrite (data.Length);
        }
    }
}
