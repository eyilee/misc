using UnityEngine;

namespace ObjectManagement
{
    namespace SpawnZones
    {
        public class GameLevel : MonoBehaviour
        {
            [SerializeField]
            SpawnZone spawnZone = null;

            void Start ()
            {
                Game.Instance.SpawnZoneOfLevel = spawnZone;
            }
        }
    }
}
