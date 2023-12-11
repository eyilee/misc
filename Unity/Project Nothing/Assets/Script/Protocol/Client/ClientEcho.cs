using ProjectNothing.Network;
using UnityEngine;

namespace ProjectNothing.Protocol
{
    public sealed class ClientEcho : NetProtocol<ClientEcho>
    {
        private string m_String;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_String);
        }

        public override void Excute ()
        {
            Debug.Log (m_String);
        }

        public override void Serialize (BitOutStream outStream)
        {
        }
    }
}
