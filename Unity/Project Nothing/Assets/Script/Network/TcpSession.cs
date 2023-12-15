using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing.Network
{
    public sealed class TcpSession
    {
        public const int TCP_SOCKET_BUFFER_SIZE = 8192;

        class ReadCommand
        {
            public int m_ByteOffset;
            public int m_Size;
            public byte[] m_Bytes;

            public ReadCommand (int size)
            {
                m_ByteOffset = 0;
                m_Size = size;
                m_Bytes = new byte[m_Size];
            }
        }

        class WriteCommand
        {
            public byte[] m_Bytes;

            public WriteCommand (BitOutStream outStream)
            {
                byte[] header = outStream.GetHeader ();
                byte[] bytes = outStream.GetBytes ();
                m_Bytes = new byte[header.Length + bytes.Length];
                header.CopyTo (m_Bytes, 0);
                bytes.CopyTo (m_Bytes, header.Length);
            }
        }

        private readonly TcpClient m_TcpClient = new ();
        private NetworkStream m_NetworkStream;

        private readonly LinkedList<ReadCommand> m_ReadQueue = new ();
        private readonly LinkedList<WriteCommand> m_WriteQueue = new ();
        private readonly byte[] m_ReadBuffer = new byte[TCP_SOCKET_BUFFER_SIZE];

        public bool Connected { get { return m_TcpClient != null && m_TcpClient.Connected; } }

        public IEnumerator Init (IPAddress ipAddress, int port)
        {
            m_TcpClient.BeginConnect (ipAddress, port, (IAsyncResult asyncResult) =>
            {
                if (!m_TcpClient.Connected)
                {
                    Debug.Log ("Server connect failed.");
                    return;
                }

                m_TcpClient.EndConnect (asyncResult);
                m_NetworkStream = m_TcpClient.GetStream ();

                Debug.Log ("Server connected.");

                AsyncRead ();
            }, null);

            yield return new WaitUntil (() => Connected);
        }

        private void AsyncRead ()
        {
            m_NetworkStream.BeginRead (m_ReadBuffer, 0, m_ReadBuffer.Length, (IAsyncResult asyncResult) =>
            {
                int length = m_NetworkStream.EndRead (asyncResult);
                OnRead (length);
                AsyncRead ();
            }, null);
        }

        private void OnRead (int length)
        {
            int offset = 0;
            int tail = length;

            if (m_ReadQueue.Last != null)
            {
                ReadCommand command = m_ReadQueue.Last.Value;
                if (command.m_Size > command.m_ByteOffset)
                {
                    int step = Math.Min (command.m_Size, tail - offset);
                    Array.Copy (m_ReadBuffer, offset, command.m_Bytes, command.m_ByteOffset, step);
                    offset += step;
                    command.m_ByteOffset += step;
                }
            }

            while (offset < tail)
            {
                int size = IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (m_ReadBuffer, offset));
                offset += sizeof (short);

                ReadCommand command = new (size);

                int step = Math.Min (command.m_Size, tail - offset);
                Array.Copy (m_ReadBuffer, offset, command.m_Bytes, command.m_ByteOffset, step);
                offset += step;
                command.m_ByteOffset += step;

                m_ReadQueue.AddLast (command);
            }

            while (m_ReadQueue.First != null)
            {
                ReadCommand command = m_ReadQueue.First.Value;
                if (command.m_Size > command.m_ByteOffset)
                {
                    break;
                }

                BitInStream inStream = new (command.m_Bytes);
                NetworkManager.ResolveInput (inStream);

                m_ReadQueue.RemoveFirst ();
            }
        }

        private void AsyncWrite ()
        {
            if (m_WriteQueue.First == null)
            {
                return;
            }

            WriteCommand command = m_WriteQueue.First ();

            m_NetworkStream.BeginWrite (command.m_Bytes, 0, command.m_Bytes.Length, (IAsyncResult asyncResult) =>
            {
                m_NetworkStream.EndWrite (asyncResult);

                m_WriteQueue.RemoveFirst ();

                AsyncWrite ();
            }, null);
        }

        public void OnWrite (BitOutStream outStream)
        {
            int size = outStream.GetSize ();
            if (size == 0 || size > TCP_SOCKET_BUFFER_SIZE)
            {
                Debug.LogErrorFormat ("Bytes size({0}) is 0 or more than {1}.", size, TCP_SOCKET_BUFFER_SIZE);
                return;
            }

            bool isSending = m_WriteQueue.Count > 0;

            m_WriteQueue.AddLast (new WriteCommand (outStream));

            if (!isSending)
            {
                AsyncWrite ();
            }
        }
    }
}
