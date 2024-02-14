using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientLoginResult : NetEvent<NE_ClientLoginResult>
    {
        public uint m_ID;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_ID);
        }

        public override void Excute ()
        {
            Debug.Log ("Logined.");
        }
    }
}
