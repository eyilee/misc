using System;
using System.Collections.Generic;
using System.Linq;

namespace SproutLands
{
    public class TileRule
    {
        public TileType TileType;
        public int Priority;
        public TileRuleCondition Condition = new ();
        public TileMaterialDefinition Definition = new ();
    }

    public class TileRuleCondition
    {
        public bool Matches (TileData tileData)
        {
            return true;
        }
    }

    public class TileMaterialDefinition
    {
        public string Name = string.Empty;
        public string Tileset = string.Empty;
        public bool IsAutoTiling = false;
        public List<int> TilesetIndices = new ();

        public int GetVariant (int x, int y)
        {
            if (TilesetIndices.Count == 0)
            {
                return 0;
            }

            int hash = (x * 73856093) ^ (y * 19349663);
            return TilesetIndices[Math.Abs (hash) % TilesetIndices.Count];
        }
    }

    public class TileRuleResolver
    {
        private readonly Dictionary<TileType, List<TileRule>> _tileRulesMap;

        public TileRuleResolver (IEnumerable<TileRule> tileRules)
        {
            _tileRulesMap = tileRules.GroupBy (x => x.TileType)
                .ToDictionary (g => g.Key, g => g.OrderByDescending (x => x.Priority).ToList ());
        }

        public TileMaterialDefinition? Resolve (TileData tileData)
        {
            if (tileData == null)
            {
                return null;
            }

            if (!_tileRulesMap.TryGetValue (tileData.Type, out List<TileRule> tileRules))
            {
                return null;
            }

            foreach (TileRule tileRule in tileRules)
            {
                if (tileRule.Condition.Matches (tileData))
                {
                    return tileRule.Definition;
                }
            }

            return null;
        }
    }
}
