using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing.Network
{
    public sealed class NetworkManager : MonoSingleton<NetworkManager>
    {
        struct Command
        {
            public byte[] m_Bytes;

            public Command (byte[] _bytes)
            {
                ushort length = Convert.ToUInt16 (_bytes.Length);
                byte[] lengthBytes = BitConverter.GetBytes (length);
                m_Bytes = new byte[lengthBytes.Length + _bytes.Length];

                Array.Copy (lengthBytes, m_Bytes, lengthBytes.Length);
                Array.Copy (_bytes, 0, m_Bytes, lengthBytes.Length, _bytes.Length);
            }
        }

        private const int SESSION_BUFFER_SIZE = 8192;

        private IPAddress m_IPAddress = null;

        private readonly TcpClient m_TcpClient = new ();
        private NetworkStream m_NetworkStream;

        private readonly UdpClient m_UdpClient = new ();
        private IPEndPoint m_RemotIPEndPoint;

        public readonly NetBridge m_NetBridge = new ();

        private readonly byte[] m_ReadBuffer = new byte[SESSION_BUFFER_SIZE];
        private readonly byte[] m_SendBuffer = new byte[SESSION_BUFFER_SIZE];

        private readonly Queue<Command> m_CommandQueue = new Queue<Command> ();

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

            yield return new WaitUntil (() => m_IsInit);

            m_RemotIPEndPoint = new IPEndPoint (m_IPAddress, _udpPort);

            AsyncReceive ();
        }

        private void AsyncRead ()
        {
            if (m_NetworkStream.CanRead)
            {
                m_NetworkStream.BeginRead (m_ReadBuffer, 0, m_ReadBuffer.Length, (IAsyncResult _asyncResult) =>
                {
                    int length = m_NetworkStream.EndRead (_asyncResult);
                    OnRead (length);
                    AsyncRead ();
                }, null);
            }
        }

        private void OnRead (int _length)
        {
            BitInStream inStream = new (m_ReadBuffer, _length);
            m_NetBridge.ResolveInput (inStream);
        }

        private void AsyncWrite ()
        {
            if (m_CommandQueue.Count == 0)
            {
                return;
            }

            Command command = m_CommandQueue.Peek ();

            m_NetworkStream.BeginWrite (command.m_Bytes, 0, command.m_Bytes.Length, (IAsyncResult _asyncResult) =>
            {
                m_NetworkStream.EndWrite (_asyncResult);

                m_CommandQueue.Dequeue ();

                AsyncWrite ();
            }, null);
        }

        public void OnWrite (BitOutStream _outStream)
        {
            byte[] bytes = _outStream.GetBytes ();
            if (bytes.Length == 0 || bytes.Length > SESSION_BUFFER_SIZE)
            {
                Debug.LogErrorFormat ("Bytes size({0}) is 0 or more than {1}.", bytes.Length, SESSION_BUFFER_SIZE);
                return;
            }

            bool isSending = m_CommandQueue.Count > 0;

            m_CommandQueue.Enqueue (new Command (bytes));

            if (!isSending)
            {
                AsyncWrite ();
            }
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
