using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace SproutLands
{
    public class WorldRenderer
    {
        public Grid? Grid { get; private set; }
        public Dictionary<int, Tilemap> Tilemaps { get; private set; }
        public Dictionary<int, TilemapRenderer> TilemapRenderers { get; private set; }

        private readonly TilesetManager _tilesetManager;

        private readonly World _world;

        public WorldRenderer (World world)
        {
            Tilemaps = new Dictionary<int, Tilemap> ();
            TilemapRenderers = new Dictionary<int, TilemapRenderer> ();

            _tilesetManager = ServiceLocator.Resolve<TilesetManager> ();

            _world = world;
        }

        public void Initialize ()
        {
            WorldData worldData = _world.Data;

            GenerateGrid (_world.Name);

            for (int height = worldData.Config.MinHeight; height <= worldData.Config.MaxHeight; height++)
            {
                GenerateTilemap ($"terrain_tilemap_{height}", TileLayerType.Terrain, height);
                GenerateTilemap ($"vegetation_tilemap_{height}", TileLayerType.Vegetation, height);
            }

            for (int x = 0; x < worldData.Width; x++)
            {
                for (int y = 0; y < worldData.Height; y++)
                {
                    TileData? tileData = worldData.GetTile (x, y);
                    if (tileData == null)
                    {
                        continue;
                    }

                    for (int height = tileData.Height; height >= worldData.Config.MinHeight; height--)
                    {
                        int mask = worldData.GetBlobTilesetMask (height, x, y);

                        SetTile (GetTilemapId (TileLayerType.Terrain, height), x, y, _tilesetManager.Resolve (tileData, mask));

                        if (!worldData.Tiles.AnyNeighbors8 (x, y, neighbor => neighbor == null || neighbor.Height < tileData.Height))
                        {
                            break;
                        }
                    }
                }
            }

            foreach ((_, Tilemap tilemap) in Tilemaps)
            {
                tilemap.CompressBounds ();
            }
        }

        private void GenerateGrid (string name)
        {
            GameObject gameObject = new (name);
            gameObject.transform.SetParent (null);
            gameObject.transform.SetLocalPositionAndRotation (Vector3.zero, Quaternion.identity);
            gameObject.transform.localScale = Vector3.one;

            Grid = gameObject.AddComponent<Grid> ();
        }

        private void GenerateTilemap (string name, TileLayerType layerType, int height)
        {
            if (Grid == null)
            {
                return;
            }

            GameObject gameObject = new (name);
            gameObject.transform.SetParent (Grid.transform);
            gameObject.transform.SetLocalPositionAndRotation (Vector3.zero, Quaternion.identity);
            gameObject.transform.localScale = Vector3.one;

            int tilemapId = GetTilemapId (layerType, height);

            Tilemap tilemap = gameObject.AddComponent<Tilemap> ();
            Tilemaps.Add (tilemapId, tilemap);

            TilemapRenderer tilemapRenderer = gameObject.AddComponent<TilemapRenderer> ();
            tilemapRenderer.sortingOrder = height;
            TilemapRenderers.Add (tilemapId, tilemapRenderer);
        }

        private int GetTilemapId (TileLayerType layerType, int height) => (int)TileLayerType.Max * height + (int)layerType;

        private void SetTile (int tilemapId, int x, int y, TileBase? tile)
        {
            if (!Tilemaps.TryGetValue (tilemapId, out Tilemap tilemap))
            {
                return;
            }

            tilemap.SetTile (new Vector3Int (x, y, 0), tile);
        }

        public void Release ()
        {
            if (Grid != null)
            {
                Object.Destroy (Grid.gameObject);
                Grid = null;
            }

            Tilemaps.Clear ();
            Tilemaps.TrimExcess ();

            TilemapRenderers.Clear ();
            TilemapRenderers.TrimExcess ();
        }
    }
}
