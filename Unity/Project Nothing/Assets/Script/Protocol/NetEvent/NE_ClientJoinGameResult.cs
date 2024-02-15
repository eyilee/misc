using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientJoinGameResult : NetEvent<NE_ClientJoinGameResult>
    {
        public uint m_GameID;
        public ushort m_Port;
        public uint m_Key;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_GameID);
            inStream.Read (out m_Port);
            inStream.Read (out m_Key);
        }

        public override void Excute ()
        {
            if (m_GameID != 0)
            {
                GameManager.JoinGame (m_GameID, m_Port, m_Key);
            }
            else
            {
                GameManager.LeaveGame ();
            }

            Debug.Log ("Joined.");
        }
    }
}
