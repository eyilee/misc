﻿using UnityEngine;

namespace ObjectManagement
{
    namespace MoreGameState
    {
        public class CompositeSpawnZone : SpawnZone
        {
            [SerializeField]
            SpawnZone[] spawnZones = null;

            [SerializeField]
            bool sequential = false;

            int nextSequentialIndex;

            public override Vector3 SpawnPoint {
                get {
                    int index;
                    if (sequential)
                    {
                        index = nextSequentialIndex++;
                        if (nextSequentialIndex >= spawnZones.Length)
                        {
                            nextSequentialIndex = 0;
                        }
                    }
                    else
                    {
                        index = Random.Range (0, spawnZones.Length);
                    }
                    return spawnZones[index].SpawnPoint;
                }
            }

            public override void Save (GameDataWriter writer)
            {
                writer.Write (nextSequentialIndex);
            }

            public override void Load (GameDataReader reader)
            {
                nextSequentialIndex = reader.ReadInt ();
            }
        }
    }
}
