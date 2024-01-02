using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientUdpConnectResult : NetEvent<NE_ClientUdpConnectResult>
    {
        public override void Deserialize (BitInStream inStream)
        {
        }

        public override void Excute ()
        {
            Debug.Log ("UdpConnected.");
        }
    }
}
