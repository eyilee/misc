using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class TcpSession
    {
        public const int TCP_SOCKET_BUFFER_SIZE = 8192;

        class ReadCommand
        {
            public int m_HeaderOffset;
            public byte[] m_Header;

            public int m_ByteOffset;
            public byte[] m_Bytes;

            public ReadCommand ()
            {
                m_HeaderOffset = 0;
                m_ByteOffset = 0;
                m_Header = new byte[2];
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

        TcpConnection m_Connection = null;
        readonly TcpClient m_TcpClient = new ();
        NetworkStream m_NetworkStream = null;

        readonly LinkedList<ReadCommand> m_ReadQueue = new ();
        readonly LinkedList<WriteCommand> m_WriteQueue = new ();
        readonly byte[] m_ReadBuffer = new byte[TCP_SOCKET_BUFFER_SIZE];

        public IEnumerator Init (TcpConnection connection, IPAddress ipAddress, int port)
        {
            m_Connection = connection;

            m_TcpClient.NoDelay = false;
            m_TcpClient.BeginConnect (ipAddress, port, (IAsyncResult asyncResult) =>
            {
                if (!m_TcpClient.Connected)
                {
                    return;
                }

                m_TcpClient.EndConnect (asyncResult);
                m_NetworkStream = m_TcpClient.GetStream ();

                AsyncRead ();
            }, null);

            yield return new WaitUntil (() => m_TcpClient.Connected);
        }

        public void Shutdown ()
        {
        }

        void AsyncRead ()
        {
            m_NetworkStream.BeginRead (m_ReadBuffer, 0, m_ReadBuffer.Length, (IAsyncResult asyncResult) =>
            {
                int length = m_NetworkStream.EndRead (asyncResult);
                OnRead (length);
                AsyncRead ();
            }, null);
        }

        void OnRead (int length)
        {
            int offset = 0;
            int tail = length;

            if (m_ReadQueue.Last != null)
            {
                ReadCommand command = m_ReadQueue.Last.Value;
                if (command.m_HeaderOffset < command.m_Header.Length)
                {
                    int step = Math.Min (command.m_Header.Length - command.m_HeaderOffset, tail - offset);
                    Array.Copy (m_ReadBuffer, offset, command.m_Header, command.m_HeaderOffset, step);
                    offset += step;
                    command.m_HeaderOffset += step;

                    if (command.m_HeaderOffset == command.m_Header.Length)
                    {
                        int size = IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (command.m_Header));
                        if (size > 0)
                        {
                            command.m_Bytes = new byte[size];
                        }
                    }
                }

                if (command.m_Bytes != null && command.m_ByteOffset < command.m_Bytes.Length)
                {
                    int step = Math.Min (command.m_Bytes.Length - command.m_ByteOffset, tail - offset);
                    Array.Copy (m_ReadBuffer, offset, command.m_Bytes, command.m_ByteOffset, step);
                    offset += step;
                    command.m_ByteOffset += step;
                }
            }

            while (offset < tail)
            {
                ReadCommand command = new ();
                if (command.m_HeaderOffset < command.m_Header.Length)
                {
                    int step = Math.Min (command.m_Header.Length - command.m_HeaderOffset, tail - offset);
                    Array.Copy (m_ReadBuffer, offset, command.m_Header, command.m_HeaderOffset, step);
                    offset += step;
                    command.m_HeaderOffset += step;

                    if (command.m_HeaderOffset == command.m_Header.Length)
                    {
                        int size = IPAddress.NetworkToHostOrder (BitConverter.ToInt16 (command.m_Header));
                        if (size > 0)
                        {
                            command.m_Bytes = new byte[size];
                        }
                    }
                }

                if (command.m_Bytes != null && command.m_ByteOffset < command.m_Bytes.Length)
                {
                    int step = Math.Min (command.m_Bytes.Length - command.m_ByteOffset, tail - offset);
                    Array.Copy (m_ReadBuffer, offset, command.m_Bytes, command.m_ByteOffset, step);
                    offset += step;
                    command.m_ByteOffset += step;
                }

                m_ReadQueue.AddLast (command);
            }

            while (m_ReadQueue.First != null)
            {
                ReadCommand command = m_ReadQueue.First.Value;
                if (command.m_HeaderOffset < command.m_Header.Length || command.m_ByteOffset < command.m_Bytes.Length)
                {
                    break;
                }

                BitInStream inStream = new (command.m_Bytes);
                m_Connection.ResolveInput (inStream);

                m_ReadQueue.RemoveFirst ();
            }
        }

        void AsyncWrite ()
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

        public void Write (BitOutStream outStream)
        {
            int size = outStream.GetSize ();
            if (size == 0 || size > TCP_SOCKET_BUFFER_SIZE)
            {
                Debug.LogErrorFormat ("Bytes size({0}) is 0 or more than TCP_SOCKET_BUFFER_SIZE({1}).", size, TCP_SOCKET_BUFFER_SIZE);
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
