using ProjectNothing.Network;
using UnityEngine;

namespace ProjectNothing.Protocol
{
    public sealed class ClientEcho : NetProtocol<ClientEcho>
    {
        private string m_String;

        public override void Deserialize (InStream inStream)
        {
            inStream.ReadString (out m_String);
        }

        public override void Excute ()
        {
            Debug.Log (m_String);
        }

        public override void Serialize (OutStream outStream)
        {
        }
    }
}
