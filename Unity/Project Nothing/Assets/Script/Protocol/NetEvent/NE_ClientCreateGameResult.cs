using UnityEngine;

namespace ProjectNothing
{
    public sealed class NE_ClientCreateGameResult : NetEvent<NE_ClientCreateGameResult>
    {
        public uint m_GameID;

        public override void Deserialize (BitInStream inStream)
        {
            inStream.Read (out m_GameID);
        }

        public override void Excute ()
        {
            ClientConnection connection = new ();

            GameManager.CreateGame (m_GameID, connection);

            NetworkManager.ComposeOutput (new NC_ServerJoinGame { m_GameID = m_GameID, m_Port = connection.Port });

            Debug.Log ("Created.");
        }
    }
}
