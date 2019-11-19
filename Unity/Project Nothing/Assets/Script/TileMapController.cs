using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace ProjectNothing
{
    public class TileMapController : MonoBehaviour
    {
        public Tilemap Tilemap { get; private set; }

        private readonly Dictionary<Vector3Int, WorldTile> m_WorldMap = new Dictionary<Vector3Int, WorldTile> ();

        public void Awake ()
        {
            Tilemap = GetComponent<Tilemap> ();
        }

        public void Start ()
        {
            foreach (Vector3Int position in Tilemap.cellBounds.allPositionsWithin)
            {
                if (!Tilemap.HasTile (position))
                {
                    continue;
                }

                TileBase tileBase = Tilemap.GetTile (position);

                if (tileBase != null && tileBase is WorldTile)
                {
                    WorldTile worldTile = Instantiate (tileBase) as WorldTile;

                    worldTile.LocalLocation = Tilemap.CellToLocal (position);
                    worldTile.WorldLocation = Tilemap.CellToWorld (position);
                    worldTile.Tilemap = Tilemap;

                    switch (worldTile.TileType)
                    {
                        case TileType.eTile_None:
                            break;
                        case TileType.eTile_Road:
                            Tilemap.SetTile (position, worldTile as RoadTile);
                            break;
                        default:
                            break;
                    }

                    m_WorldMap.Add (position, worldTile);
                }
            }
        }

        public void OnPointerDown (Vector2 point)
        {
            Vector3Int cell = Tilemap.WorldToCell (point);

            Debug.Log (cell);

            OnTileClick (cell);
        }

        private void OnTileClick (Vector3Int position)
        {
            if (m_WorldMap.ContainsKey (position))
            {
                WorldTile worldTile = m_WorldMap[position];

                worldTile.OnClick ();
            }
        }
    }
}
