using ProjectNothing.Network;
using UnityEngine;

namespace ProjectNothing.Protocol
{
    sealed class ClientEcho : NetProtocol<ClientEcho>
    {
        private string kString;

        public override void Deserialize (InStream inStream)
        {
            inStream.ReadString (out kString);
        }

        public override void Excute ()
        {
            Debug.Log (kString);
        }

        public override void Serialize (OutStream outStream)
        {
        }
    }
}
