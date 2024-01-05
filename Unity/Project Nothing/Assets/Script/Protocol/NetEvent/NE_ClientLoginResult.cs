using System.Net;
using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientLoginResult : NetEvent<NE_ClientLoginResult>
    {
        public uint m_ID;
        public uint m_Key;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_ID);
            inStream.Read (out m_Key);
        }

        public override void Excute ()
        {
            NetworkManager.m_ID = m_ID;
            NetworkManager.m_Key = m_Key;

            IPEndPoint endPoint = NetworkManager.GetUdpIPEndPoint ();
            NetworkManager.ComposeTcpOutput (new NC_ServerUdpConnect { m_nUdpPort = (short)endPoint.Port });

            Debug.Log ("Logined.");
        }
    }
}
