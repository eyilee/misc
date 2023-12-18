using ProjectNothing.Network;
using UnityEngine;

namespace ProjectNothing.Protocol
{
    public sealed class NE_ClientEchoResult : NetEvent<NE_ClientEchoResult>
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
    }
}
