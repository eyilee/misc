using UnityEngine;

namespace SproutLands
{
    public class MapGenerator
    {
        public bool GenerateMap (WorldData worldData, WorldConfig config)
        {
            if (config.Width <= 0 || config.Height <= 0)
            {
                return false;
            }

            float xOffset = Random.value * config.Width;
            float xScale = 1f / 10f;
            float yOffset = Random.value * config.Height;
            float yScale = 1f / 10f;

            Map<int> heightMap = new (config.Width, config.Height);

            for (int x = 0; x < heightMap.Width; x++)
            {
                for (int y = 0; y < heightMap.Height; y++)
                {
                    heightMap.SetValue (x, y, Mathf.RoundToInt (Mathf.Clamp01 (Mathf.PerlinNoise (x * xScale + xOffset, y * yScale + yOffset)) * config.MaxHeight));
                }
            }

            for (int iteration = 0; iteration < 5; iteration++)
            {
                bool hasChanged = false;

                for (int x = 0; x < heightMap.Width; x++)
                {
                    for (int y = 0; y < heightMap.Height; y++)
                    {
                        int minAllowed = 0;
                        int maxAllowed = config.MaxHeight;

                        heightMap.ForEachNeighbors8 (x, y, neighborHeight =>
                        {
                            minAllowed = Mathf.Max (minAllowed, neighborHeight - 1);
                            maxAllowed = Mathf.Min (maxAllowed, neighborHeight + 1);
                        });

                        int height = heightMap.GetValue (x, y);
                        int clamped = Mathf.Clamp (height, minAllowed, maxAllowed);
                        if (clamped != height)
                        {
                            heightMap.SetValue (x, y, clamped);
                            hasChanged = true;
                        }
                    }
                }

                if (!hasChanged)
                {
                    break;
                }
            }

            for (int x = 0; x < worldData.Tiles.Width; x++)
            {
                for (int y = 0; y < worldData.Tiles.Height; y++)
                {
                    int height = heightMap.GetValue (x, y);
                    TileFlag flags = TileFlag.None;

                    if (!heightMap.AnyNeighbors8 (x, y, neighborHeight => height > neighborHeight))
                    {
                        flags |= TileFlag.Walkable;
                    }

                    worldData.Tiles.SetValue (x, y, new TileData (x, y)
                    {
                        Height = height,
                        Flags = flags
                    });
                }
            }

            return true;
        }
    }
}
