using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class ClientLoginResult : NetProtocol<ClientLoginResult>
    {
        public override void Deserialize (BitInStream inStream)
        {
        }

        public override void Excute ()
        {
            GameManager.Instance.m_Login = true;
        }

        public override void Serialize (BitOutStream outStream)
        {
        }
    }
}
