using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class WorldTile : Tile
{
    //[Space (20)]
    //[Header ("World Tile")]
    public TileType TileType { get; set; }

    public Vector3 LocalLocation { get; set; }

    public Vector3 WorldLocation { get; set; }

    public Tilemap Tilemap { get; set; }

    public override bool StartUp (Vector3Int position, ITilemap tilemap, GameObject go)
    {
        return base.StartUp (position, tilemap, go);
    }

    public virtual void OnClick () { }
}
