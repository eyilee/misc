using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace SproutLands
{
    public class TilesetManager : Service<TilesetManager>
    {
        private readonly List<TilesetDefinition> _tilesetDefinitions = new ()
        {
            new TilesetDefinition
            {
                Name = "blue_tint_Grass",
                Path = "Tilemaps/blue_tint_Grass/blue_tint_Grass"
            },
            new TilesetDefinition
            {
                Name = "blue_tint_Grass_flowers",
                Path = "Tilemaps/blue_tint_Grass_flowers/blue_tint_Grass_flowers"
            }
        };

        private readonly Dictionary<string, Tileset> _tilesets = new ();

        private readonly List<TileRule> _tileRules = new ()
        {
            new TileRule ()
            {
                TileType = TileType.Terrain,
                Priority = 0,
                Condition = new TileRuleCondition (),
                Definition = new TileMaterialDefinition ()
                {
                    Name = "blue_tint_Grass",
                    Tileset = "blue_tint_Grass",
                    IsAutoTiling = true
                }
            },
            new TileRule ()
            {
                TileType = TileType.Plant,
                Priority = 0,
                Condition = new TileRuleCondition (),
                Definition = new TileMaterialDefinition ()
                {
                    Name = "blue_tint_Grass_flowers",
                    Tileset = "blue_tint_Grass_flowers",
                    IsAutoTiling = false,
                    TilesetIndices = new () { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 }
                }
            }
        };

        private readonly TileRuleResolver _tileRuleResolver;

        public TilesetManager ()
        {
            _tileRuleResolver = new TileRuleResolver (_tileRules);
        }

        public void Initialize ()
        {
            foreach (TilesetDefinition tilesetDefinition in _tilesetDefinitions)
            {
                Tileset tileset = Resources.Load<Tileset> (tilesetDefinition.Path);
                if (tileset != null)
                {
                    _tilesets.Add (tilesetDefinition.Name, tileset);
                }
            }
        }

        public TileBase? Resolve (TileData tileData, int mask)
        {
            TileMaterialDefinition? definition = _tileRuleResolver.Resolve (tileData);
            if (definition == null)
            {
                return null;
            }

            if (!_tilesets.TryGetValue (definition.Tileset, out Tileset tileset))
            {
                return null;
            }

            return tileset.Resolve (definition.GetVariant (tileData.X, tileData.Y), mask);
        }
    }
}
