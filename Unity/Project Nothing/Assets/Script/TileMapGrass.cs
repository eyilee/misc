using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class TileMapGrass : MonoBehaviour
{
    Tilemap tilemap = null;

    readonly Dictionary<Vector3Int, WorldTile> worldMap = new Dictionary<Vector3Int, WorldTile> ();

    public void Awake ()
    {
        tilemap = GetComponent<Tilemap> ();
    }

    public void Start ()
    {
        foreach (Vector3Int position in tilemap.cellBounds.allPositionsWithin)
        {
            if (!tilemap.HasTile (position))
            {
                continue;
            }

            TileBase tileBase = tilemap.GetTile (position);

            if (tileBase != null && tileBase is WorldTile)
            {
                WorldTile worldTile = Instantiate (tileBase) as WorldTile;

                worldTile.LocalLocation = tilemap.CellToLocal (position);
                worldTile.WorldLocation = tilemap.CellToWorld (position);
                worldTile.Tilemap = tilemap;

                switch (worldTile.TileType)
                {
                    case TileType.eTile_None:
                        break;
                    case TileType.eTile_Road:
                        tilemap.SetTile (position, worldTile as RoadTile);
                        break;
                    default:
                        break;
                }

                worldMap.Add (position, worldTile);
            }
        }
    }

    public void OnTileClick (Vector3Int position)
    {
        TileBase tileBase = tilemap.GetTile (position);

        if (tileBase != null && tileBase is WorldTile)
        {
            WorldTile worldTile = tileBase as WorldTile;

            worldTile.OnClick ();
        }
    }
}
