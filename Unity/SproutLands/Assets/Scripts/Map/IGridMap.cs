using UnityEngine;

namespace SproutLands
{
    public interface IGridMap
    {
        Vector3 GetCellCenterWorld (Vector3Int coord);

        bool CanEnter (Vector3Int coord);

        public Vector3Int GetSpawnCoord ();

        public TileData? GetTileData (Vector3Int coord);
    }
}
