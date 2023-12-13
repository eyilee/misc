using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class ClientLoginResult : NetProtocol<ClientLoginResult>
    {
        public uint m_Key;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_Key);
        }

        public override void Excute ()
        {
            GameManager.Instance.m_Login = true;
            GameManager.Instance.m_Key = m_Key;
        }

        public override void Serialize (BitOutStream outStream)
        {
        }
    }
}
