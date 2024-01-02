using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientLoginResult : NetEvent<NE_ClientLoginResult>
    {
        public int m_ID;
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
            NetworkManager.ComposeUdpOutput (new NC_ServerUdpConnect ());

            Debug.Log ("Logined.");
        }
    }
}
