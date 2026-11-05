using System.Collections.Generic;

namespace SproutLands
{
    public class WorldData
    {
        public readonly WorldConfig Config;
        public readonly Map<TileData> Tiles;

        public int Width => Config.Width;
        public int Height => Config.Height;

        private readonly IdAllocator _idAllocator = new ();
        private readonly Dictionary<int, TileObject> _objects = new ();

        public WorldData (WorldConfig config)
        {
            Config = config;
            Tiles = new Map<TileData> (config.Width, config.Height);
        }

        public TileData? GetTile (int x, int y) => Tiles.GetValue (x, y);

        public TileData? GetTile (Coord coord) => Tiles.GetValue (coord.X, coord.Y);

        public bool InBounds (int x, int y) => Tiles.InBounds (x, y);

        public bool InBounds (Coord coord) => Tiles.InBounds (coord.X, coord.Y);

        public bool AddObject (TileObject tileObject)
        {
            tileObject.Id = _idAllocator.Allocate ();

            return _objects.TryAdd (tileObject.Id, tileObject);
        }

        public int GetBlobTilesetMask (int height, int x, int y)
        {
            int mask = 0;

            for (int i = 0; i < Coord.Neighbors8.Length; i++)
            {
                Coord neighborCoord = Coord.Neighbors8[i];

                if (!Tiles.TryGetValue (x + neighborCoord.X, y + neighborCoord.Y, out TileData? neighbor))
                {
                    mask |= 1 << i;
                    continue;
                }

                if (neighbor == null || neighbor.Height >= height)
                {
                    mask |= 1 << i;
                }
            }

            return mask;
        }
    }
}
