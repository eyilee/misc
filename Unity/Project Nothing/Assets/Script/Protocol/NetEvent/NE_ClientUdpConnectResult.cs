using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientUdpConnectResult : NetEvent<NE_ClientUdpConnectResult>
    {
        public bool m_Result;
        public ushort m_Port;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_Result);
            inStream.Read (out m_Port);
        }

        public override void Excute ()
        {
            if (m_Result)
            {
                NetworkManager.SetupUdpConnection (m_Port);
                Debug.Log ("UdpConnected.");
            }
            else
            {
                Debug.Log ("UdpConnect failed.");
            }
        }
    }
}
