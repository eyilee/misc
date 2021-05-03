using UnityEngine;

namespace ObjectManagement
{
    namespace MoreGameState
    {
        public abstract class SpawnZone : PersistableObject
        {
            public abstract Vector3 SpawnPoint { get; }
        }
    }
}
