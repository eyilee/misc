using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class ClientLoginResult : NetProtocol<ClientLoginResult>
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
            GameManager.Instance.m_Login = true;
            NetworkManager.m_ID = m_ID;
            NetworkManager.m_Key = m_Key;
            NetworkManager.ComposeUdpOutput (new ServerUdpConnect ());
        }

        public override void Serialize (BitOutStream outStream)
        {
        }
    }
}
