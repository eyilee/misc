using ProjectNothing.Network;

namespace ProjectNothing.Protocol
{
    public sealed class NE_ClientCreateGameResult : NetEvent<NE_ClientCreateGameResult>
    {
        public override void Deserialize (BitInStream inStream)
        {
        }

        public override void Excute ()
        {
            GameManager.Instance.CreateGame ();
        }
    }
}
