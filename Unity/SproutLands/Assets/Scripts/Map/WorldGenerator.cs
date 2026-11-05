using UnityEngine;

namespace SproutLands
{
    public class WorldGenerator
    {
        private readonly MapGenerator _mapGenerator = new ();
        private readonly ObjectGenerator _objectGenerator = new ();

        public World? GenerateWorld (string name, WorldConfig config)
        {
            Random.InitState (config.Seed);

            WorldData worldData = new (config);

            if (!_mapGenerator.GenerateMap (worldData, config))
            {
                return null;
            }

            if (!_objectGenerator.GenerateObjects (worldData, config))
            {
                return null;
            }

            return new World (name, worldData);
        }
    }
}
