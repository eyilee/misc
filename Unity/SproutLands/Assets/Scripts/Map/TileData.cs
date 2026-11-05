using System;
using System.Collections.Generic;

namespace SproutLands
{
    public enum TileType
    {
        Terrain,
        Plant
    }

    [Flags]
    public enum TileFlag
    {
        None = 0,
        Walkable
    }

    public class TileData
    {
        public readonly int X;
        public readonly int Y;

        public int Height;
        public TileType Type;
        public TileFlag Flags;
        public List<int> Objects = new ();

        public TileData (int x, int y)
        {
            X = x;
            Y = y;
        }

        public bool IsWalkable => Flags.HasFlag (TileFlag.Walkable);
    }
}
