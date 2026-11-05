using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace SproutLands
{
    [CreateAssetMenu (fileName = "Tileset", menuName = "Scriptable Objects/Tileset")]
    public class Tileset : ScriptableObject
    {
        public class TileEntry
        {
            public readonly TileBase Tile;

            public TileEntry (TileBase tile)
            {
                Tile = tile;
            }
        }

        public TileBase[] Tiles = default!;

        [NonSerialized]
        protected bool _isInit = false;

        [NonSerialized]
        protected TileEntry[]? _tileEntries;

        public virtual TilesetType TilesetType => TilesetType.None;

        public virtual void Initialize ()
        {
            if (_isInit)
            {
                return;
            }

            _tileEntries = Tiles.Select (t => new TileEntry (t))
                .ToArray ();

            _isInit = true;
        }

        public virtual TileBase? Resolve (int index, int mask)
        {
            return GetTile (index);
        }

        public virtual TileBase? GetTile (int index)
        {
            Initialize ();

            if (_tileEntries == null || index < 0 || index >= _tileEntries.Length)
            {
                return null;
            }

            return _tileEntries[index]?.Tile;
        }
    }
}
