using UnityEngine;

namespace ProjectNothing
{
    [CreateAssetMenu (fileName = "Config", menuName = "ScriptableObjects/GameConfig", order = 1)]
    public class GameConfig : ScriptableObject
    {
        public string Host;
        public ushort Port;

        public ushort TickRate;
    }
}
