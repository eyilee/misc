using UnityEngine;

namespace ObjectManagement
{
    namespace SpawnZones
    {
        public abstract class SpawnZone : MonoBehaviour
        {
            public abstract Vector3 SpawnPoint { get; }
        }
    }
}
