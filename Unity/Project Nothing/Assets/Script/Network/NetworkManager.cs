using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing.Network
{
    public sealed class NetworkManager : MonoSingleton<NetworkManager>
    {
        private const int SESSION_BUFFER_SIZE = 8192;

        private IPAddress m_IPAddress = null;

        private readonly TcpClient m_TcpClient = new ();
        private NetworkStream m_NetworkStream;

        private readonly UdpClient m_UdpClient = new ();
        private IPEndPoint m_RemotIPEndPoint;

        public readonly NetBridge m_NetBridge = new ();

        private readonly byte[] m_ReadBuffer = new byte[SESSION_BUFFER_SIZE];
        private readonly byte[] m_WriteBuffer = new byte[SESSION_BUFFER_SIZE];
        private readonly byte[] m_SendBuffer = new byte[SESSION_BUFFER_SIZE];

        private bool m_IsInit = false;

        public IEnumerator Init (string _host, int _tcpPort, int _udpPort)
        {
            if (!IPAddress.TryParse (_host, out m_IPAddress))
            {
                yield break;
            }

            m_TcpClient.BeginConnect (m_IPAddress, _tcpPort, (IAsyncResult _asyncResult) =>
            {
                if (!m_TcpClient.Connected)
                {
                    Debug.Log ("Server connect failed.");
                    return;
                }

                m_TcpClient.EndConnect (_asyncResult);
                m_NetworkStream = m_TcpClient.GetStream ();
                m_IsInit = true;

                Debug.Log ("Server connected.");

                AsyncRead ();
            }, null);

            m_RemotIPEndPoint = new IPEndPoint (m_IPAddress, _udpPort);

            AsyncReceive ();

            yield return new WaitUntil (() => m_IsInit);
        }

        private void AsyncRead ()
        {
            if (m_NetworkStream.CanRead)
            {
                m_NetworkStream.BeginRead (m_WriteBuffer, 0, m_WriteBuffer.Length, (IAsyncResult _asyncResult) =>
                {
                    int length = m_NetworkStream.EndRead (_asyncResult);
                    OnRead (length);
                    AsyncRead ();
                }, null);
            }
        }

        private void OnRead (int _length)
        {
            BitInStream inStream = new (m_WriteBuffer, _length);
            m_NetBridge.ResolveInput (inStream);
        }

        private void AsyncWrite (int _length)
        {
            if (m_NetworkStream.CanWrite)
            {
                m_NetworkStream.BeginWrite (m_ReadBuffer, 0, _length, (IAsyncResult _asyncResult) =>
                {
                    m_NetworkStream.EndWrite (_asyncResult);
                }, null);
            }
        }

        public void OnWrite (BitOutStream _outStream)
        {
            byte[] bytes = _outStream.GetBytes ();
            Array.Copy (bytes, m_ReadBuffer, bytes.Length);
            AsyncWrite (bytes.Length);
        }

        private void AsyncReceive ()
        {
            m_UdpClient.BeginReceive ((IAsyncResult _asyncResult) =>
            {
                byte[] bytes = m_UdpClient.EndReceive (_asyncResult, ref m_RemotIPEndPoint);
                OnReceive (bytes);
                AsyncReceive ();
            }, null);
        }

        private void OnReceive (byte[] _bytes)
        {
            BitInStream inStream = new (_bytes);
            inStream.Read (out uint key);

            if (key == GameManager.Instance.m_Key)
            {
                m_NetBridge.ResolveInput (inStream);
            }
        }

        private void AsyncSend (int _length)
        {
            m_UdpClient.BeginSend (m_SendBuffer, _length, m_RemotIPEndPoint, (IAsyncResult _asyncResult) =>
            {
                m_UdpClient.EndSend (_asyncResult);
            }, null);
        }

        public void OnSend (BitOutStream _outStream)
        {
            byte[] bytes = _outStream.GetBytes ();
            Array.Copy (bytes, m_SendBuffer, bytes.Length);
            AsyncSend (bytes.Length);
        }
    }
}
