using UnityEngine;

namespace SproutLands
{
    public class World : IGridMap
    {
        public string Name;
        public WorldData Data;
        public WorldRenderer Renderer;

        public World (string name, WorldData worldData)
        {
            Name = name;
            Data = worldData;
            Renderer = new WorldRenderer (this);
        }

        public void Initialize ()
        {
            Renderer.Initialize ();
        }

        public void Release ()
        {
            Renderer.Release ();
        }

        public bool CanEnter (Vector3Int coord)
        {
            TileData? tileData = GetTileData (coord);
            return tileData != null && tileData.IsWalkable;
        }

        public Vector3 GetCellCenterWorld (Vector3Int coord)
        {
            return Renderer.Grid != null ? Renderer.Grid.GetCellCenterWorld (coord) : Vector3.zero;
        }

        public Vector3Int GetSpawnCoord ()
        {
            for (int i = 0; i < 20; i++)
            {
                int x = Random.Range (0, Data.Tiles.Width);
                int y = Random.Range (0, Data.Tiles.Height);

                TileData? tileData = Data.Tiles.GetValue (x, y);
                if (tileData == null)
                {
                    continue;
                }

                if (tileData.IsWalkable)
                {
                    return new Vector3Int (x, y, 0);
                }
            }

            return Vector3Int.zero;
        }

        public TileData? GetTileData (Vector3Int coord)
        {
            return Data.Tiles.GetValue (coord.x, coord.y);
        }
    }
}
