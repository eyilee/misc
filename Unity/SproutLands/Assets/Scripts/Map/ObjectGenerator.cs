using System.Collections.Generic;
using System.Linq;
using UnityEngine;

namespace SproutLands
{
    public class ObjectGenerator
    {
        public bool GenerateObjects (WorldData worldData, WorldConfig config)
        {
            List<ObjectRule> rules = config.ObjectRules.OrderByDescending (x => x.Priority).ToList ();

            for (int x = 0; x < worldData.Width; x++)
            {
                for (int y = 0; y < worldData.Height; y++)
                {
                    foreach (ObjectRule rule in rules)
                    {
                        TileData? tileData = worldData.GetTile (x, y);
                        if (tileData == null)
                        {
                            continue;
                        }

                        if (MatchedRule (tileData, rule))
                        {
                            if (rule.UseCluster)
                            {
                                GenerateClusters (worldData, x, y, rule);
                            }
                            else
                            {
                                GenerateSimple (worldData, x, y, rule);
                            }
                        }
                    }
                }
            }

            return true;
        }

        private static bool MatchedRule (TileData tileData, ObjectRule rule)
        {
            if (rule.UseHeightLimit && (tileData.Height < rule.MinHeight || tileData.Height > rule.MaxHeight))
            {
                return false;
            }

            if (rule.RequireWalkable && !tileData.Flags.HasFlag (TileFlag.Walkable))
            {
                return false;
            }

            return true;
        }

        private void GenerateSimple (WorldData worldData, int x, int y, ObjectRule rule)
        {
            if (Random.value > rule.BaseChance)
            {
                return;
            }

            worldData.AddObject (new TileObject () { Type = rule.Type, Coord = new Coord (x, y) });
        }

        private void GenerateClusters (WorldData worldData, int x, int y, ObjectRule rule)
        {
            if (Random.value > rule.BaseChance)
            {
                return;
            }

            Coord start = new (x, y);

            Queue<Coord> queue = new ();
            HashSet<Coord> cluster = new ();

            queue.Enqueue (start);
            cluster.Add (start);

            int targetSize = Random.Range (rule.MinClusterSize, rule.MaxClusterSize);

            while (queue.Count > 0 && cluster.Count < targetSize)
            {
                Coord current = queue.Dequeue ();

                worldData.AddObject (new TileObject () { Type = rule.Type, Coord = current });

                foreach (Coord neighbor in Coord.Neighbors4)
                {
                    Coord next = current + neighbor;

                    if (!worldData.InBounds (next))
                    {
                        continue;
                    }

                    if (cluster.Contains (next))
                    {
                        continue;
                    }

                    TileData? nextTileData = worldData.GetTile (next);
                    if (nextTileData == null)
                    {
                        continue;
                    }

                    if (!MatchedRule (nextTileData, rule))
                    {
                        continue;
                    }

                    float priority = ComputePriority (next, cluster);

                    if (Random.value > priority)
                    {
                        continue;
                    }

                    queue.Enqueue (next);
                    cluster.Add (next);
                }
            }
        }

        private float ComputePriority (Coord coord, HashSet<Coord> cluster)
        {
            int neighbors = 0;

            foreach (Coord neighbor in Coord.Neighbors4)
            {
                if (cluster.Contains (coord + neighbor))
                {
                    neighbors++;
                }
            }

            return neighbors / 4f;
        }
    }
}