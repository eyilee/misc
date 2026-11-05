using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace SproutLands
{
    [CreateAssetMenu (fileName = "Blob Tileset", menuName = "Scriptable Objects/Blob Tileset")]
    public class BlobTileset : Tileset
    {
        private const int FullMask = 0xFF;

        public class BlobTileEntry : TileEntry
        {
            public readonly int RequiredMask;

            public BlobTileEntry (TileBase tile) : base (tile)
            {
                if (!TryParseMask (tile.name, out int parsedMask))
                {
                    Debug.LogError ($"Invalid tile name format: {tile.name}");
                    parsedMask = 0;
                }

                RequiredMask = NormalizeMask (parsedMask);
            }
        }

        [NonSerialized]
        protected BlobTileEntry[]? _blobTileEntries;

        public override TilesetType TilesetType => TilesetType.Blob;

        public override void Initialize ()
        {
            if (_isInit)
            {
                return;
            }

            _blobTileEntries = Tiles.Select (t => new BlobTileEntry (t))
                .OrderByDescending (t => PopCount (t.RequiredMask))
                .ToArray ();

            _tileEntries = _blobTileEntries;

            _isInit = true;
        }

        public override TileBase? Resolve (int index, int mask)
        {
            return GetTileByMask (mask);
        }

        public TileBase? GetTileByMask (int mask)
        {
            Initialize ();

            if (_blobTileEntries == null)
            {
                return null;
            }

            mask = NormalizeMask (mask);

            return _blobTileEntries.FirstOrDefault (t => (mask & t.RequiredMask) == t.RequiredMask)?.Tile;
        }

        private static bool TryParseMask (string name, out int mask)
        {
            int underscoreIndex = name.LastIndexOf ('_');
            if (underscoreIndex < 0)
            {
                mask = 0;
                return false;
            }

            return int.TryParse (name[(underscoreIndex + 1)..], out mask);
        }

        private static int NormalizeMask (int mask)
        {
            if ((mask & 1) == 0 || (mask & 4) == 0)
            {
                mask &= ~2; // NE
            }

            if ((mask & 4) == 0 || (mask & 16) == 0)
            {
                mask &= ~8; // SE
            }

            if ((mask & 16) == 0 || (mask & 64) == 0)
            {
                mask &= ~32; // SW
            }

            if ((mask & 64) == 0 || (mask & 1) == 0)
            {
                mask &= ~128; // NW
            }

            return mask & FullMask;
        }

        private static int PopCount (int value)
        {
            value &= FullMask;

            int count = 0;
            while (value != 0)
            {
                value &= (value - 1);
                count++;
            }

            return count;
        }
    }
}
