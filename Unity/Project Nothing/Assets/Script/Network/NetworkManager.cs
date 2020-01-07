using System;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing.Network
{
    sealed class NetworkManager : MonoSingleton<NetworkManager>
    {
        private readonly TcpClient tcpClient = new TcpClient ();
        private NetworkStream networkStream;

        public readonly NetBridge netBridge = new NetBridge ();

        private readonly byte[] sendBuffer = new byte[1024];
        private readonly byte[] receiveBuffer = new byte[1024];

        public void Init (string host, int port)
        {
            tcpClient.BeginConnect (host, port, OnConnect, null);
        }

        private void OnConnect (IAsyncResult asyncResult)
        {
            tcpClient.EndConnect (asyncResult);

            if (tcpClient.Connected)
            {
                networkStream = tcpClient.GetStream ();

                AsyncRead ();

                Debug.Log ("Server connected.");
            }
        }

        private void AsyncRead ()
        {
            if (networkStream.CanRead)
            {
                networkStream.BeginRead (receiveBuffer, 0, receiveBuffer.Length, (IAsyncResult asyncResult) =>
                {
                    int length = networkStream.EndRead (asyncResult);
                    OnRead (length);
                    AsyncRead ();
                }, null);
            }
        }

        private void OnRead (int length)
        {
            InStream inStream = new InStream (receiveBuffer, length);
            netBridge.ResolveInput (inStream);
        }

        private void AsyncWrite (int length)
        {
            if (networkStream.CanWrite)
            {
                networkStream.BeginWrite (sendBuffer, 0, length, (IAsyncResult asyncResult) =>
                {
                    networkStream.EndWrite (asyncResult);
                }, null);
            }
        }

        public void OnWrite (OutStream outStream)
        {
            byte[] data = outStream.Data ();
            Array.Copy (data, sendBuffer, data.Length);
            AsyncWrite (data.Length);
        }
    }
}
