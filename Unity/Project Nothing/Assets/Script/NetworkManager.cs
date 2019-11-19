using System;
using System.Net.Sockets;
using UnityEngine;

namespace ProjectNothing
{
    public class NetworkManager : MonoSingleton<NetworkManager>
    {
        private readonly TcpClient tcpClient = new TcpClient ();

        private void Start ()
        {
            tcpClient.BeginConnect ("127.0.0.1", 3000, OnConnect, null);

            //IPAddress.HostToNetworkOrder ();
            //IPAddress.NetworkToHostOrder ();
        }

        private void OnConnect (IAsyncResult asyncResult)
        {
            if (tcpClient.Connected)
            {
                Debug.Log ("Server connected.");
            }

            NetworkStream networkStream = tcpClient.GetStream ();

            if (networkStream.CanWrite)
            {
                string message = "This is a test.";

                byte[] data = System.Text.Encoding.ASCII.GetBytes (message);

                networkStream.Write (data, 0, data.Length);
            }

            if (networkStream.CanRead)
            {
                byte[] data = new byte[128];

                int length = networkStream.Read (data, 0, data.Length);

                string message = System.Text.Encoding.ASCII.GetString (data, 0, length);

                Debug.Log (message);
            }

            tcpClient.EndConnect (asyncResult);

            tcpClient.Close ();
        }
    }
}
